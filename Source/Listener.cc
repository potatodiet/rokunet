#include "Listener.h"

namespace rokunet {

Listener::Listener() {
    setTimeout(0);
}

bool Listener::accept(Socket* socket) noexcept {
    socket->handle = ::accept(handle,
                              static_cast<sockaddr*>(nullptr),
                              static_cast<socklen_t*>(nullptr));
    return true;
}

bool Listener::listen(unsigned short port) noexcept {
    auto address = createAddress("0.0.0.0", port);
    if (bind(handle,
             reinterpret_cast<sockaddr*>(&address),
             sizeof(address)) == 0) {
        return !::listen(handle, 0);
    } else {
        return false;
    }
}

} // namespace rokunet

