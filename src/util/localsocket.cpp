#include "localsocket.hpp"
#include <cstring>
#include <thread> // std::this_thread::sleep
#include <sys/socket.h> // socket
#include <sys/un.h> // sockaddr_un
#include <unistd.h> // close

int LocalSocket::Connect(const char* address, uint32_t retry_count, uint32_t timeout_ms) {
    fd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (fd == -1)
        return -1;

    sockaddr_un address_struct;
    address_struct.sun_family = AF_LOCAL;
    std::strcpy(address_struct.sun_path, address);

    uint32_t sleep_ms = timeout_ms / (retry_count + 1);
    for (size_t i = 0;; ++i) {
        if (connect(fd, (sockaddr*)&address_struct, sizeof(address_struct)) != -1)
            return 0;

        if (i >= retry_count)
            break;
        std::this_thread::sleep_for(std::chrono::milliseconds{sleep_ms});
    }

    Close();
    return -1;
}

int LocalSocket::Close() {
    if (fd != -1)
        return close(fd);
    return 0;
}
