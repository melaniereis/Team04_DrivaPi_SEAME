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

int main(int argc, char** argv) {
    // Parse command-line arguments
    std::string can_interface = "can0";
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

    // Main read loop
    while (true) {
        can_frame frame;
        const ssize_t nbytes = read(can_sock, &frame, sizeof(frame));

        if (nbytes < 0) {
            if (errno == EINTR) {
                continue;  // Interrupted by signal, retry
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

    close(can_sock);
    std::cout << "[Feeder] Stopped." << std::endl;
    return 0;
}
