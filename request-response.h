//
// Created by zebulon on 11/10/16.
//

#ifndef CPPONNECT_REQUEST_RESPONSE_H
#define CPPONNECT_REQUEST_RESPONSE_H

#include <string>
#include <vector>
#include <map>
#include <boost/asio.hpp>

namespace cpponnect {
    struct request {
        std::map<std::string, std::string> headers;
        std::string httpVersion;
        int httpVersionMajor;
        int httpVersionMinor;
        std::string method;
        std::vector<std::string> rawHeaders;
        std::string url;

        std::map<std::string, void *> properties;
    };

    struct response {
        void end();
        void end(std::string chunk, std::string encoding);
        void end(char *chunk, size_t length);
        bool finished = false;
        std::string getHeader(std::string name);
        bool headersSent = false;
        void removeHeader(std::string name);
        bool sendDate;
        void setHeader(std::string name, std::string value);
        int statusCode = 200;
        std::string statusMessage;
        bool write(std::string chunk, std::string encoding);
        bool write(char *chunk, size_t length);
        void writeContinue();
        void writeHead(int statusCode);
        void writeHead(int statusCode, std::string statusMessage);
        void writeHead(int statusCode, std::map<std::string, std::string> headers);
        void writeHead(int statusCode, std::string statusMessage, std::map<std::string, std::string> headers);

        response(boost::asio::ip::tcp::socket &socket);

        std::map<std::string, void *> properties;

    private:
        boost::asio::ip::tcp::socket &socket;
        std::map<std::string, std::string> headers;
    };
};


#endif //CPPONNECT_REQUEST_RESPONSE_H