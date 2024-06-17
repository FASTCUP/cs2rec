#pragma once
#include <cstdint>

/// Connects and closes a named local socket
struct LocalSocket {
    /**
     * @param address Typically a filesystem address like /tmp/mysocket
     * @param retry_count Number of retries to perform during the timeout period
     * @param timeout_ms Timeout in milliseconds
     */
    int Connect(const char* address, uint32_t retry_count, uint32_t timeout_ms);
    /// Returns 0, else -1 on error
    int Close();
    /// Socket fd
    int fd = -1;

    LocalSocket() {}
    LocalSocket(LocalSocket&&) = delete;
    LocalSocket(const LocalSocket&) = delete;
    ~LocalSocket() { Close(); }
};