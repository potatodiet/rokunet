#include "Socket.h"

#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

namespace rokunet {

Socket::Socket(const Domain domain) {
    handle = socket(domainToInt(domain), SOCK_STREAM, 0);
    setTimeout(5);
}

Socket::~Socket() {
    disconnect();
}

void Socket::send(const std::string& data) noexcept {
    ::send(handle, data.data(), data.size(), 0);
}

std::string Socket::receive(const unsigned int size) noexcept {
    char buffer[size];
    auto received = recv(handle, buffer, size, 0);
    return std::string(buffer, received);
}

bool Socket::connect(const std::string& host,
                     const unsigned short port) noexcept {
    auto address = createAddress(host, port);
    return ::connect(handle,
                     reinterpret_cast<sockaddr*>(&address),
                     sizeof(address)) == 0;
}

bool Socket::disconnect() noexcept {
    return ::close(handle) == 0;
}

sockaddr_in Socket::createAddress(const std::string& host,
                                  const unsigned short port) const noexcept {
    sockaddr_in address;
    std::memset(&address, 0, sizeof(address));

    inet_pton(AF_INET, host.data(), &(address.sin_addr));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);

    return address;
}

void Socket::setTimeout(const long seconds, const long microseconds) noexcept {
    struct timeval tv;
    tv.tv_sec = seconds;
    tv.tv_usec = microseconds;
    setsockopt(
        handle, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval)
    );
}

void Socket::setTimeout(const long seconds) noexcept {
    setTimeout(seconds, 0);
}

std::string Socket::getRemoteAddress() const noexcept {
    sockaddr_in address;

    socklen_t size = sizeof(address);
    getpeername(handle, reinterpret_cast<sockaddr*>(&address), &size);

    char buffer[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &address.sin_addr, buffer, sizeof(buffer));

    return std::string(buffer);
}

unsigned short Socket::getLocalPort() const noexcept {
    sockaddr_in address;
    socklen_t size = sizeof(address);

    getsockname(handle, reinterpret_cast<sockaddr*>(&address), &size);

    return ntohs(address.sin_port);
}

unsigned short Socket::domainToInt(const Domain domain) {
    int cDomain;
    switch (domain) {
    case Local:
        cDomain = AF_LOCAL;
        break;
    case Inet:
        cDomain = AF_INET;
        break;
    case Inet6:
        cDomain = AF_INET6;
        break;
    case IPX:
        cDomain = AF_IPX;
        break;
    case Netlink:
        cDomain = AF_NETLINK;
        break;
    case X25:
        cDomain = AF_X25;
        break;
    case AX25:
        cDomain = AF_AX25;
        break;
    case ATMPVC:
        cDomain = AF_ATMPVC;
        break;
    case AppleTalk:
        cDomain = AF_APPLETALK;
        break;
    case Packet:
        cDomain = AF_PACKET;
        break;
    case ALG:
        cDomain = AF_ALG;
        break;
    }
    return cDomain;
}

} // namespace rokunet

