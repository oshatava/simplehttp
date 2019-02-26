#include "logger.h"
#include "server.h"
#include "domain.h"
#include "response_fabric.h"

int main(){
    logger::Logger::create(logger::DEBUG);

    // Init http callbacks
    server::ResponseFabric::get()->addRoute(METHOD_GET, "/version", server::createT<server::VersionResponse>);
    server::ResponseFabric::get()->addRoute(METHOD_GET, "/devices", server::createT<server::DevicesResponse>);
    server::ResponseFabric::get()->addRoute(METHOD_GET, "/device/([0-9]+)", server::createT<server::DeviceResponse>);

    // Init server
    server::Server server(8081, 10);
    
    // Start server
    server.start();
    server.join();
    logger::Logger::destroy();
    return 0;
}