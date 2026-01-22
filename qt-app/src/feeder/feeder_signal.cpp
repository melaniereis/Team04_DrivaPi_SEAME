#include "feeder_signal.hpp"
#include <signal.h>
#include <sys/wait.h>
#include <thread>
#include <mutex>
#include <algorithm>
#include <iostream>
#include <errno.h>
#include <string.h>

namespace feeder {

// Global stop flag for main loop
std::atomic<bool> g_stop_requested(false);

// Registry of child PIDs (not modified in signal handlers to avoid async-unsafe issues)
static std::vector<pid_t> g_child_pids;
static std::mutex g_child_pids_mutex;

/**
 * @brief SIGINT/SIGTERM handler - sets stop flag to break main loop
 */
static void handle_termination(int /*signum*/) {
    g_stop_requested.store(true);
}

/**
 * @brief SIGCHLD handler - reaps zombie child processes
 * @note Kept minimal as signal handlers should be
 */
static void handle_child_exit(int /*signum*/) {
    // Save errno to restore after
    int saved_errno = errno;
    
    // Reap all available dead children (WNOHANG = don't block)
    while (true) {
        pid_t pid = waitpid(-1, nullptr, WNOHANG);
        if (pid <= 0) break;  // No more children to reap
    }
    
    errno = saved_errno;
}

void InstallSignalHandlers()
{
    struct sigaction sa;
    
    // --- SIGINT/SIGTERM: graceful shutdown ---
    std::memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle_termination;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;  // Don't set SA_RESTART so blocking read() gets EINTR
    
    sigaction(SIGINT, &sa, nullptr);
    sigaction(SIGTERM, &sa, nullptr);
    
    // --- SIGCHLD: reap children to avoid zombies ---
    struct sigaction sa_chld;
    std::memset(&sa_chld, 0, sizeof(sa_chld));
    sa_chld.sa_handler = handle_child_exit;
    sigemptyset(&sa_chld.sa_mask);
    sa_chld.sa_flags = SA_NOCLDSTOP;  // Don't trigger for SIGSTOP/SIGCONT
    
    sigaction(SIGCHLD, &sa_chld, nullptr);
}

void RegisterChildPid(pid_t pid)
{
    if (pid <= 0) return;
    
    std::lock_guard<std::mutex> lock(g_child_pids_mutex);
    if (std::find(g_child_pids.begin(), g_child_pids.end(), pid) == g_child_pids.end()) {
        g_child_pids.push_back(pid);
    }
}

void KillRegisteredChildren()
{
    std::lock_guard<std::mutex> lock(g_child_pids_mutex);
    
    if (g_child_pids.empty()) return;
    
    std::cout << "[Signal] Terminating " << g_child_pids.size() << " child process(es)..." << std::endl;
    
    // Step 1: Send SIGTERM to all children
    for (pid_t pid : g_child_pids) {
        if (pid > 0) {
            kill(pid, SIGTERM);
        }
    }
    
    // Step 2: Wait briefly for graceful shutdown
    const int wait_ms = 200;
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = wait_ms * 1000000;
    nanosleep(&ts, nullptr);
    
    // Step 3: Force kill any stragglers
    for (pid_t pid : g_child_pids) {
        if (pid > 0) {
            // Check if still alive before killing
            if (kill(pid, 0) == 0) {  // 0 signal just checks if process exists
                kill(pid, SIGKILL);
            }
        }
    }
    
    g_child_pids.clear();
}

} // namespace feeder
