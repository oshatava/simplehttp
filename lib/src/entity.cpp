#include "entity.h"
#include "logger.h"
#include "task.h"
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>
#include <regex>

using namespace server;

inline std::string codeDescr(int code)
{
    switch (code)
    {
    case 200:
        return "OK";
    case 400:
        return "Bad Request";
    case 404:
        return "Not Found";
    case 500:
        return "Internal Server Error";
    default:
        return "";
    }
}

std::string Response::build()
{
    std::stringstream stream;
    std::string body = this->body.str();
    int contentLength = body.length();
    stream << "HTTP/1.1 " << retCode << " " << codeDescr(retCode) << std::endl;
    stream << "Content-Length :" << contentLength << std::endl;
    std::map<std::string, std::string>::iterator it;
    for (it = headers.begin(); it != headers.end(); it++)
    {
        stream << it->first << " :" << it->second << std::endl;
    }
    stream << std::endl;
    stream << body;
    return stream.str();
}
