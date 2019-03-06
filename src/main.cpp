#include "logger.h"
#include "server.h"
#include "domain.h"
#include "configuration.h"
#include "coder.h"
#include "session.h"

using namespace domain;
using namespace http_logger;

#ifndef APP_SERVER_NAME
#define APP_SERVER_NAME u8"Олег server"
#endif


int main()
{
    logger::Logger::create(logger::DEBUG);
    session::SessionManager::create();
    // Init http callbacks
    server::Configuration configuration(8080, 10);

    configuration
        .route(METHOD_GET, "/version", VersionResponse)
        .route(METHOD_GET, "/devices", DevicesResponse)
        .route(METHOD_GET, "/device/([0-9]+)", DeviceResponse)
        .error(RESPONSE_CODE_ERROR_404, My404ErrorPage)
        .preProccessor(logRequest)
        .preProccessor(security::RequestDeCoder)
        .preProccessor(session::SessionProvider)
        .postProccessor([](server::Response &response) {
            response.setHeader("Server", APP_SERVER_NAME);
        })
        .postProccessor(session::SessionMerge)
        .postProccessor(logResponse)
        .postProccessor(security::ResponseEnCoder)
        ;

    // Init server
    server::Server server(configuration);

    // Start server
    server.start();
    server.join();
    session::SessionManager::destroy();
    logger::Logger::destroy();
    return 0;
}