#ifndef CPPONNECT_SERVER_H
#define CPPONNECT_SERVER_H

#include "middleware.h"

namespace cpponnect {
    class server {
        middleware_t on_request;
    public:
        // `next` is `nullptr` when middleware called by server.
        server(middleware_t on_request);

        void listen(int port);

        bool listening = false;
    };
};


#endif //CPPONNECT_APP_H
