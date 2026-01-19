/**
 * @file main.cpp
 * @brief KUKSA CAN Feeder - Main entry point
 * 
 * Reads CAN frames from SocketCAN and publishes VSS signals to KUKSA databroker
 * 
 * Usage: kuksa_feeder [can_interface] [kuksa_address] [options]
 *   can_interface: CAN interface name (default: can0)
 *   kuksa_address: KUKSA databroker address (default: localhost:55555)
 *   options:
 *     --insecure            Use insecure channel (default)
 *     --tls                 Use TLS channel
 *     --ca <path>           Root CA certificate path
 *     --cert <path>         Client certificate (for mTLS)
 *     --key <path>          Client private key (for mTLS)
 *     --token <jwt>         Authorization token (adds Authorization: Bearer <jwt>)
 */

#include "publisher.hpp"
#include "handlers.hpp"
#include "can_ids.hpp"
#include <iostream>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <signal.h>
#include <sys/wait.h>
#include <vector>
#include <mutex>
#include <algorithm>

/**
 * @brief Open SocketCAN interface
 * @param ifname Interface name (e.g., "can0", "vcan0")
 * @return Socket file descriptor, or -1 on error
 */
static int OpenCanSocket(const std::string& ifname) {
    // Create CAN socket
    int sock = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (sock < 0) {
        std::cerr << "[CAN] Failed to create socket: " << std::strerror(errno) << std::endl;
        return -1;
    }

    // Get interface index
    struct ifreq ifr;
    std::memset(&ifr, 0, sizeof(ifr));
    std::strncpy(ifr.ifr_name, ifname.c_str(), IFNAMSIZ - 1);

    if (ioctl(sock, SIOCGIFINDEX, &ifr) < 0) {
        std::cerr << "[CAN] Failed to get interface index for " << ifname 
                  << ": " << std::strerror(errno) << std::endl;
        close(sock);
        return -1;
    }

    // Bind socket to CAN interface
    struct sockaddr_can addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(sock, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) < 0) {
        std::cerr << "[CAN] Failed to bind socket to " << ifname 
                  << ": " << std::strerror(errno) << std::endl;
        close(sock);
        return -1;
    }

    std::cout << "[CAN] Listening on interface: " << ifname << std::endl;
    return sock;
}

/**
 * @brief Dispatch CAN frame to appropriate handler
 * @param frame The received CAN frame
 * @param publisher KUKSA publisher instance
 */
static void DispatchFrame(const can_frame& frame, kuksa::Publisher& publisher) {
    const uint32_t can_id = frame.can_id & CAN_SFF_MASK;  // Mask to standard frame ID

    switch (can_id) {
        case can::ID_SPEED:
            handlers::HandleSpeed(frame, publisher);
            break;

        // Add additional CAN IDs here:
        // case can::ID_TEMPERATURE:
        //     handlers::handleTemperature(frame, publisher);
        //     break;

        default:
            // Ignore unknown CAN IDs (or log if debugging)
            // std::cout << "[CAN] Ignoring unknown ID: 0x" << std::hex << can_id << std::dec << std::endl;
            break;
    }
}

static volatile sig_atomic_t g_stop_requested = 0;

static void signal_handler(int /*signum*/) {
    g_stop_requested = 1;
}

// Reap child processes to avoid zombies. Keep this handler minimal.
static void sigchld_handler(int /*signum*/) {
    int saved_errno = errno;
    // Reap all dead children
    while (true) {
        pid_t pid = waitpid(-1, nullptr, WNOHANG);
        if (pid <= 0) break;
    }
    errno = saved_errno;
}

// Optional registry for child PIDs spawned by this process. Use
// `RegisterChildPid(pid)` after fork/exec so the shutdown routine can
// attempt to terminate them. This registry is NOT modified in signal
// handlers.
static std::vector<pid_t> g_child_pids;
static std::mutex g_child_pids_mutex;

static void RegisterChildPid(pid_t pid) {
    if (pid <= 0) return;
    std::lock_guard<std::mutex> lk(g_child_pids_mutex);
    if (std::find(g_child_pids.begin(), g_child_pids.end(), pid) == g_child_pids.end()) {
        g_child_pids.push_back(pid);
    }
}

static void KillRegisteredChildren() {
    std::lock_guard<std::mutex> lk(g_child_pids_mutex);
    for (pid_t pid : g_child_pids) {
        if (pid <= 0) continue;
        // First try polite termination
        kill(pid, SIGTERM);
    }
    // Give them a short chance to exit
    const int wait_ms = 200;
    struct timespec ts{0, wait_ms * 1000000};
    nanosleep(&ts, nullptr);

    for (pid_t pid : g_child_pids) {
        if (pid <= 0) continue;
        // If still alive, force kill
        kill(pid, SIGKILL);
    }
}

int main(int argc, char** argv) {
    // Parse command-line arguments
    std::string can_interface = "vcan0";
    std::string kuksa_address = "localhost:55555";
    kuksa::PublisherOptions popts;
    popts.address = kuksa_address;

    if (argc >= 2) {
        can_interface = argv[1];
    }
    if (argc >= 3) {
        kuksa_address = argv[2];
        popts.address = kuksa_address;
    }

    // Parse optional flags from argv[3..]
    for (int i = 3; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--insecure") {
            popts.use_ssl = false;
        } else if (arg == "--tls") {
            popts.use_ssl = true;
        } else if (arg == "--ca" && i + 1 < argc) {
            popts.root_ca_path = argv[++i];
        } else if (arg == "--cert" && i + 1 < argc) {
            popts.client_cert_path = argv[++i];
        } else if (arg == "--key" && i + 1 < argc) {
            popts.client_key_path = argv[++i];
        } else if (arg == "--token" && i + 1 < argc) {
            popts.token = argv[++i];
        } else {
            std::cerr << "[Args] Unknown or incomplete option: " << arg << std::endl;
        }
    }

    std::cout << "========================================" << std::endl;
    std::cout << "  KUKSA CAN Feeder" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "CAN Interface: " << can_interface << std::endl;
    std::cout << "KUKSA Address: " << kuksa_address << std::endl;
    std::cout << "Security: " << (popts.use_ssl ? "TLS" : "insecure") << std::endl;
    if (!popts.root_ca_path.empty()) std::cout << "CA: " << popts.root_ca_path << std::endl;
    if (!popts.client_cert_path.empty()) std::cout << "Cert: " << popts.client_cert_path << std::endl;
    if (!popts.client_key_path.empty()) std::cout << "Key: " << popts.client_key_path << std::endl;
    if (!popts.token.empty()) std::cout << "Auth: Bearer <token>" << std::endl;
    std::cout << "========================================" << std::endl;

    // Connect to KUKSA databroker
    kuksa::Publisher publisher(popts);

    // Open CAN socket
    const int can_sock = OpenCanSocket(can_interface);
    if (can_sock < 0) {
        std::cerr << "[Error] Failed to open CAN interface. Is it up?" << std::endl;
        std::cerr << "        Try: sudo ip link set " << can_interface << " up" << std::endl;
        return 1;
    }

    std::cout << "[Feeder] Running. Press Ctrl+C to stop." << std::endl;

    // Install signal handlers for graceful shutdown
    struct sigaction sa;
    std::memset(&sa, 0, sizeof(sa));
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0; // do not set SA_RESTART so read() is interrupted by signals
    sigaction(SIGINT, &sa, nullptr);
    sigaction(SIGTERM, &sa, nullptr);

    // Install SIGCHLD handler to reap children and avoid zombies
    struct sigaction sa_chld;
    std::memset(&sa_chld, 0, sizeof(sa_chld));
    sa_chld.sa_handler = sigchld_handler;
    sigemptyset(&sa_chld.sa_mask);
    sa_chld.sa_flags = SA_NOCLDSTOP;
    sigaction(SIGCHLD, &sa_chld, nullptr);

    // Main read loop
    while (true) {
        can_frame frame;
        const ssize_t nbytes = read(can_sock, &frame, sizeof(frame));

        if (nbytes < 0) {
            if (errno == EINTR) {
                if (g_stop_requested) {
                    // Received SIGINT/SIGTERM: break out to shutdown gracefully
                    break;
                }
                // Interrupted by other signal, retry
                continue;
            }
            std::cerr << "[CAN] Read error: " << std::strerror(errno) << std::endl;
            break;
        }

        if (nbytes != sizeof(frame)) {
            std::cerr << "[CAN] Incomplete frame read: " << nbytes << " bytes" << std::endl;
            continue;
        }

        // Dispatch to handler
        DispatchFrame(frame, publisher);
    }

    // Attempt to terminate any registered child processes
    KillRegisteredChildren();

    close(can_sock);
    std::cout << "[Feeder] Stopped." << std::endl;
    return 0;
}
