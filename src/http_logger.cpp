#include "logger.h"
#include "entity.h"
#include "domain.h"

void http_logger::logResponse(server::Response &response)
{
    int retCode = response.getRetCode();
    logger::Logger::d() << "Response ret code is - " << retCode << logger::endl;
    for (auto header : response.getHeaders())
    {
        logger::Logger::d() << "Header [" << header.first.c_str() << "] = " << header.second.c_str() << logger::endl;
    }
}

void http_logger::logRequest(server::Request &request)
{
    logger::Logger::d() << "Method is " << request.getMethod().c_str() << logger::endl;
    logger::Logger::d() << "Path is " << request.getPath().c_str() << logger::endl;

    for (auto header : request.getHeaders())
    {
        logger::Logger::d() << "Header [" << header.first.c_str() << "] = " << header.second.c_str() << logger::endl;
    }
    for (auto parameter : request.getParamsGet())
    {
        logger::Logger::d() << "Get [" << parameter.first.c_str() << "] = " << parameter.second.c_str() << logger::endl;
    }
    for (auto parameter : request.getParamsPath())
    {
        logger::Logger::d() << "Path [" << parameter.first.c_str() << "] = " << parameter.second.c_str() << logger::endl;
    }
    for (auto parameter : request.getParamsPost())
    {
        logger::Logger::d() << "Post [" << parameter.first.c_str() << "] = " << parameter.second.c_str() << logger::endl;
    }
}
