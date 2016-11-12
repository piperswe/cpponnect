#include "app.h"
#include <boost/algorithm/string/predicate.hpp>

using namespace cpponnect;

void app::use(middleware_t middleware) {
    installed_middleware.push_back(middleware);
}

void app::use(std::string mount_point, middleware_t middleware) {
    if (mount_point == "/") {
        use(middleware);
    } else {
        mounted_middleware.push_back({ mount_point, middleware });
    }
}

void app::use(error_middleware_t middleware) {
    installed_error_middleware.push_back(middleware);
}

void app::use(std::string mount_point, error_middleware_t middleware) {
    if (mount_point == "/") {
        use(middleware);
    } else {
        mounted_error_middleware.push_back({ mount_point, middleware });
    }
}

bool mount_point_matches(std::string mount_point, std::string url) {
    return boost::starts_with(url, mount_point);
}

void app::operator()(request &req, response &res) {
    try {
        for (const auto &middleware : installed_middleware) {
            middleware(req, res);
            if (res.finished) return;
        }

        for (const auto &middleware : mounted_middleware) {
            if (mount_point_matches(middleware.mount_point, req.url)) {
                middleware.value(req, res);
            }
            if (res.finished) return;
        }

        res.end();
    } catch (std::exception exception) {
        for (const auto &middleware : installed_error_middleware) {
            middleware(exception, req, res);
            if (res.finished) return;
        }

        for (const auto &middleware : mounted_error_middleware) {
            if (mount_point_matches(middleware.mount_point, req.url)) {
                middleware.value(exception, req, res);
            }
            if (res.finished) return;
        }

        res.end();
    }
}