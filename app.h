#ifndef CPPONNECT_APP_H
#define CPPONNECT_APP_H

#include <vector>
#include "middleware.h"

namespace cpponnect {
    template<typename T>
    struct mounted {
        std::string mount_point;
        T value;
    };

    class app : public middleware_t {
        std::vector<middleware_t> installed_middleware;
        std::vector<mounted<middleware_t>> mounted_middleware;
        std::vector<error_middleware_t> installed_error_middleware;
        std::vector<mounted<error_middleware_t>> mounted_error_middleware;
    public:
        void use(middleware_t middleware);
        void use(error_middleware_t middleware);
        void use(std::string mount_point, middleware_t middleware);
        void use(std::string mount_point, error_middleware_t middleware);

        void operator()(cpponnect::request &req, cpponnect::response &res);
    };
};


#endif //CPPONNECT_APP_H
