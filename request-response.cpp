//
// Created by zebulon on 11/10/16.
//

#include "request-response.h"

using namespace cpponnect;

static inline std::string messageOf(int code) {
    switch (code) {
        case 100: return "Continue";
        case 101: return "Switching Protocols";
        case 200: return "OK";
        case 201: return "Created";
        case 202: return "Accepted";
        case 203: return "Non-Authoritative Information";
        case 204: return "No Content";
        case 205: return "Reset Content";
        case 206: return "Partial Content";
        case 300: return "Multiple Choices";
        case 301: return "Moved Permanently";
        case 302: return "Found";
        case 303: return "See Other";
        case 304: return "Not Modified";
        case 305: return "Use Proxy";
        case 307: return "Temporary Redirect";
        case 400: return "Bad Request";
        case 401: return "Unauthorized";
        case 402: return "Payment Required";
        case 403: return "Forbidden";
        case 404: return "Not Found";
        case 405: return "Method Not Allowed";
        case 406: return "Not Acceptable";
        case 407: return "Proxy Authentication Required";
        case 408: return "Request Timeout";
        case 409: return "Conflict";
        case 410: return "Gone";
        case 411: return "Length Required";
        case 412: return "Precondition Failed";
        case 413: return "Request Entity Too Large";
        case 414: return "Request-URI Too Large";
        case 415: return "Unsupported Media Type";
        case 416: return "Requested range not satisfiable";
        case 500: return "Internal Server Error";
        case 501: return "Not Implemented";
        case 502: return "Bad Gateway";
        case 503: return "Service Unavailable";
        case 504: return "Gateway Time-out";
        case 505: return "HTTP Version not supported";
        default: return "";
    }
}

response::response(boost::asio::ip::tcp::socket &socket) : socket(socket) { }

void response::end() {
    if (finished) return;
    finished = true;
    boost::system::error_code err;
    socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, err);
    if (err) throw err;
    socket.close();
}

void response::end(std::string chunk, std::string encoding) {
    if (finished) return;
    write(chunk, encoding);
    end();
}

void response::end(char *chunk, size_t length) {
    if (finished) return;
    write(chunk, length);
    end();
}

std::string response::getHeader(std::string name) {
    if (finished) return nullptr;
    if (headersSent) return nullptr;
    return headers[name];
}

void response::removeHeader(std::string name) {
    if (finished) return;
    if (headersSent) return;
    headers.erase(name);
}

void response::setHeader(std::string name, std::string value) {
    if (finished) return;
    if (headersSent) return;
    headers[name] = value;
}

bool response::write(std::string chunk, std::string encoding) {
    if (finished) return false;
    if (!headersSent) writeHead(statusCode);
    return chunk.length() == 0 || socket.write_some(boost::asio::buffer(chunk)) != 0;
}

bool response::write(char *chunk, size_t length) {
    if (finished) return false;
    if (!headersSent) writeHead(statusCode);
    return length == 0 || socket.write_some(boost::asio::buffer(chunk, length)) != 0;
}

void response::writeContinue() {
    if (finished) return;
    if (headersSent) return;
    socket.write_some(boost::asio::buffer(std::string("HTTP/1.1 100 Continue\r\n\r\n")));
}

void response::writeHead(int statusCode) {
    if (finished) return;
    if (headersSent) return;
    this->statusCode = statusCode;
    statusMessage = messageOf(statusCode);
    writeHead(statusCode, statusMessage, headers);
}

void response::writeHead(int statusCode, std::string statusMessage) {
    if (finished) return;
    if (headersSent) return;
    this->statusCode = statusCode;
    this->statusMessage = statusMessage;
    writeHead(statusCode, statusMessage, headers);
}

void response::writeHead(int statusCode, std::map<std::string, std::string> headers) {
    if (finished) return;
    if (headersSent) return;
    this->statusCode = statusCode;
    this->headers = headers;
    writeHead(statusCode, statusMessage, headers);
}

void response::writeHead(int statusCode, std::string statusMessage, std::map<std::string, std::string> headers) {
    if (finished) return;
    if (headersSent) return;
    this->statusCode = statusCode;
    this->statusMessage = statusMessage;
    this->headers = headers;

    std::stringstream ss;
    ss << "HTTP/1.1 ";
    ss << statusCode;
    ss << " ";
    ss << statusMessage;
    ss << "\r\n";
    for (auto const &header : headers) {
        ss << header.first;
        ss << ": ";
        ss << header.second;
        ss << "\r\n";
    }
    ss << "\r\n";

    socket.write_some(boost::asio::buffer(ss.str()));
}
