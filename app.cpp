#include "app.h"
#include <boost/algorithm/string/predicate.hpp>

using namespace cpponnect;

bool mount_point_matches(std::string mount_point, std::string url) {
    return boost::starts_with(url, mount_point);
}

void app::use(middleware_t middleware) {
    installed_middleware.push_back(middleware);
}

void app::use(std::string mount_point, middleware_t middleware) {
    use([mount_point, middleware](auto &x, auto &y) {
        if (mount_point_matches(mount_point, x.url)) middleware(x, y);
    });
}

void app::use(error_middleware_t middleware) {
    installed_error_middleware.push_back(middleware);
}

void app::use(std::string mount_point, error_middleware_t middleware) {
    use([mount_point, middleware](auto x, auto &y, auto &z) {
        if (mount_point_matches(mount_point, y.url)) middleware(x, y, z);
    });
}

void app::operator()(request &req, response &res) {
    try {
        for (const auto &middleware : installed_middleware) {
            middleware(req, res);
            if (res.finished) return;
        }

        res.end();
    } catch (std::exception exception) {
        for (const auto &middleware : installed_error_middleware) {
            middleware(exception, req, res);
            if (res.finished) return;
        }

        res.end();
    }
}