#include "configuration.h"
#include <regex>
#include <sstream>
#include <logger.h>
#include <http.h>

using namespace server;

inline Response err404Response(Request &request)
{
    return Response(request)
        .setHeader(HEADER_CONTENT_TYPE, CONTENT_TYPE_TEXT)
        .setBody(html::HTML()
        .tag("head")
            .tag("meta").param("http-equiv", "Content-Type").end()
            .tag("title").body("404 Error").end()
        .end()
        .tag("body")
            .tag("h1").body("404 Error").end()
            .tag("p")
                .body("Page - '")
                .tag("b").body(request.getPath()).end()
                .body("' not found")
            .end()
        .end()
        .build())
        .setRetCode(RESPONSE_CODE_ERROR_404);
}

inline Response err500Response(Request &request, std::exception &e)
{
    return Response(request)
        .setHeader(HEADER_CONTENT_TYPE, CONTENT_TYPE_TEXT)
        .setBody(html::HTML()
        .tag("head")
            .tag("meta").param("http-equiv", "Content-Type").end()
            .tag("title").body("500 Error").end()
        .end()
        .tag("body")
            .tag("h1").body("500 Error").end()
            .tag("p")
                .body("Error thrown:")
                .tag("br").end()
                .tag("p").body(e.what()).end()
            .end()
        .end()
        .build())
        .setRetCode(RESPONSE_CODE_ERROR_500);
}

Configuration::Configuration(int port, int maxConnection)
{
    this->port = port;
    this->maxConnection = maxConnection;
    preProccessorRaw(http::requestProvider);
    error(RESPONSE_CODE_ERROR_404, err404Response);
    error(err500Response);
}

Configuration &Configuration::route(std::string method, std::string path, ResponseProvider responseProvider)
{
    callbackCreatorMap[method][path] = responseProvider;
    return (*this);
}

Configuration &Configuration::error(int error, ResponseProvider responseProvider)
{
    errorCreatorMap[error] = responseProvider;
    return (*this);
}
Configuration &Configuration::error(ResponseExceptionProvider responseProvider)
{
    responseExceptionProvider = responseProvider;
    return (*this);
}

Configuration &Configuration::preProccessor(RequestProccesor preProccessor)
{
    preProccessors.push_back(preProccessor);
    return (*this);
}

Configuration &Configuration::postProccessor(ResponseProccesor postProccessor)
{
    postProccessors.push_back(postProccessor);
    return (*this);
}

Configuration &Configuration::preProccessorRaw(RequestProvider requestProvider)
{
    preProccessorRawFunc = requestProvider;
    return (*this);
}

Response Configuration::createResponse(const std::vector<char> &data)
{
    Request request = this->preProccessorRawFunc(data);
    try
    {
        std::string method = request.getMethod();
        std::string path = request.getPath();
        std::map<std::string, ResponseProvider> methods = callbackCreatorMap[method];
        bool gotResponse = false;
        ResponseProvider responseProvider = NULL;

        for (std::pair<std::string, ResponseProvider> entry : methods)
        {
            std::regex rPath(entry.first);
            std::smatch match;
            if (std::regex_match(request.getPath(), match, rPath))
            {
                for (int i = 0; i < match.size(); i++)
                {
                    std::stringstream index;
                    index << "$" << i;
                    request.getParamsPath()[index.str()] = match[i];
                }
                if (entry.second != NULL)
                {
                    responseProvider = entry.second;
                    gotResponse = true;
                    break;
                }
            }
        }

        if (responseProvider == NULL)
        {
            ResponseProvider errorPageCreatorFn = errorCreatorMap[RESPONSE_CODE_ERROR_404];
            if (errorPageCreatorFn != NULL)
            {
                responseProvider = errorPageCreatorFn;
            }
            else
            {
                responseProvider = err404Response;
            }
        }

        for (auto requestProccesor : this->preProccessors)
        {
            requestProccesor(request);
        }

        Response response = responseProvider(request);

        for (auto responseProccesor : this->postProccessors)
        {
            responseProccesor(response);
        }
        return response;
    }
    catch (server::ServerError &e)
    {
        logger::Logger::e()<<"Error thrown(ServerError):"<<e.what()<<logger::endl;
        return responseExceptionProvider(request, e);
    }    
    catch (std::exception &e)
    {
        logger::Logger::e()<<"Error thrown(exception):"<<e.what()<<logger::endl;
        return responseExceptionProvider(request, e);
    }    
    catch (const char *errorMessage)
    {
        server::ServerError err(errorMessage);
        logger::Logger::e()<<"Error thrown(const char *):"<<errorMessage<<logger::endl;
        return responseExceptionProvider(request, err);
    }    
}
