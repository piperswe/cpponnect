#ifndef CPPONNECT_MIDDLEWARE_H
#define CPPONNECT_MIDDLEWARE_H

#include <functional>
#include "request-response.h"

namespace cpponnect {
    typedef std::function<void(cpponnect::request &, cpponnect::response &)> middleware_t;
    typedef std::function<void(std::exception, cpponnect::request &, cpponnect::response &)> error_middleware_t;
};


#endif //CPPONNECT_MIDDLEWARE_H
