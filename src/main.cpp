#include "logger.h"
#include "server.h"
#include "domain.h"
#include "configuration.h"
#include "coder.h"

using namespace domain;
using namespace http_logger;

int main()
{
    logger::Logger::create(logger::DEBUG);

    // Init http callbacks
    server::Configuration configuration(8081, 10);

    configuration
        .route(METHOD_GET, "/version", VersionResponse)
        .route(METHOD_GET, "/devices", DevicesResponse)
        .route(METHOD_GET, "/device/([0-9]+)", DeviceResponse)
        .error(RESPONSE_CODE_ERROR_404, My404ErrorPage)
        .preProccessor(logRequest)
        .preProccessor(security::RequestDeCoder)
        .postProccessor([](server::Response &response) {
            response.setHeader("Server", SERVER_NAME);
        })
        .postProccessor([](server::Response &response) {
            response.setHeader("Encrypted", "1");
        })
        .postProccessor(logResponse)
        .postProccessor(security::ResponseEnCoder)
        ;

    // Init server
    server::Server server(configuration);

    // Start server
    server.start();
    server.join();
    logger::Logger::destroy();
    return 0;
}