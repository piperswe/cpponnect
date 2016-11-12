#include <fstream>
#include "server.h"
#include "app.h"

using namespace cpponnect;

int main(void) {
    app my_app;

    std::string index = R"INDEXHTML(
<!DOCTYPE html>
<html>
    <head>
        <title>Cpponnect</title>
    </head>
    <body>
        <strong>Cpponnect</strong> seems to be working fine!
    </body>
</html>
    )INDEXHTML";

    my_app.use("/headers", [](auto &req, auto &res) {
        res.write("Here's your headers:\n", "utf-8");
        for (const auto &header : req.headers) {
            res.write(header.first, "utf-8");
            res.write(": ", "utf-8");
            res.write(header.second, "utf-8");
            res.write("\n", "utf-8");
        }
        res.end("You're welcome!", "utf-8");
    });

    my_app.use([index](auto &req, auto &res) {
        res.end(index, "utf-8");
    });

    std::cout << "Listening on port 3000" << std::endl;
    server(my_app).listen(3000);
}