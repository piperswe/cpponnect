//
// Created by zebulon on 11/10/16.
//

#include "server.h"
#include <boost/array.hpp>

using boost::asio::ip::tcp;

using namespace cpponnect;

server::server(middleware_t on_request) : on_request(on_request) { }

request parse_request(std::string req) {
    request parsed;
    size_t i = 0;

    // Grab the method
    {
        std::stringstream ss;
        for (; i < req.length() && req[i] != ' '; i++) {
            ss << req[i];
        }
        parsed.method = ss.str();
        i++;
    }

    // Grab the URL
    {
        std::stringstream ss;
        for (; i < req.length() && req[i] != ' '; i++) {
            ss << req[i];
        }
        parsed.url = ss.str();
        i++;
    }

    // Parse the version number
    {
        std::stringstream ss;
        i += 5;
        std::stringstream major;
        std::stringstream minor;
        bool on_minor = false;
        for (; i < req.length() && req[i] != '\r' && req[i] != '\n'; i++) {
            ss << req[i];
            if (on_minor) {
                minor << req[i];
            } else if (req[i] != '.') {
                major << req[i];
            } else {
                on_minor = true;
            }
        }
        parsed.httpVersion = ss.str();
        parsed.httpVersionMajor = std::stoi(major.str());
        parsed.httpVersionMinor = std::stoi(minor.str());
        if (req[i + 1] == '\n')
            i += 2;
        else
            i++;
    }

    // Parse the headers
    {
        while (i < req.length()) {
            std::stringstream header_name;
            std::stringstream header_value;

            for (; i < req.length() && req[i] != ':'; i++) {
                header_name << req[i];
            }

            i += 2;

            for (; i < req.length() && req[i] != '\r' && req[i] != '\n'; i++) {
                header_value << req[i];
            }

            parsed.rawHeaders.push_back(header_name.str());
            parsed.rawHeaders.push_back(header_value.str());
            parsed.headers[header_name.str()] = header_value.str();
            if (req[i + 1] == '\n')
                i += 2;
            else
                i++;
        }
    }

    return parsed;
}

void server::listen(int port) {
    boost::asio::io_service io_service;
    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), port));
    listening = true;

    while (this->listening) {
        tcp::socket socket(io_service);
        acceptor.accept(socket);

        boost::array<char, 128> buf;
        boost::system::error_code err;

        size_t len = socket.read_some(boost::asio::buffer(buf), err);
        if (err) socket.write_some(boost::asio::buffer(err.message()));

        request req = parse_request(std::string(buf.data()));
        response res(socket);

        on_request(req, res);
    }

    acceptor.close();
}