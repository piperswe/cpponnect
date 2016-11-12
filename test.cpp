#include "server.h"
#include "app.h"

using namespace cpponnect;

int main(void) {
    app my_app;

    my_app.use([](auto &req, auto &res) {
        throw std::exception();
//        res.end(std::string("Hello, world!\n"), std::string("utf-8"));
    });

    my_app.use([](auto err, auto &req, auto &res) {
        res.statusCode = 500;
        res.end(std::string("Oh, no! Something went wrong!\n"), std::string("utf-8"));
    });

    std::cout << "Listening on port 3000" << std::endl;
    server(my_app).listen(3000);
}