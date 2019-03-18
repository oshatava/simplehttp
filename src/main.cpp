#include "logger.h"
#include "server.h"
#include "domain.h"
#include "configuration.h"
#include "coder.h"
#include "session.h"
#include "imdb.h"

using namespace domain;
using namespace http_logger;

#ifndef APP_SERVER_NAME
#define APP_SERVER_NAME u8"Олег server"
#endif

int main()
{
    logger::Logger::create(logger::DEBUG);
    session::SessionManager::create();
    imdb::Imdb::create();    
    // Init http callbacks
    server::Configuration configuration(8080, 10);
    
    imdb::DataContainer<std::string, Device> deviceContainer;
    imdb::DataSource<Device, domain::D2S, domain::S2D, domain::DeviceMatcher> deviceDS(deviceContainer);
    imdb::MemoryDB devices(deviceDS);
    for(int i=0; i<1000; i++){
        std::stringstream id;
        id<<"dev"<<i;
        deviceContainer.setItem(id.str(), domain::Device(id.str(), i%100));
    }
   
    configuration
        .route(METHOD_GET, "/version", VersionResponse)
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

    imdb::setUpHttpCallbacks(configuration, "/dev", &devices);
    
    // Init server
    server::Server server(configuration);

    // Start server
    server.start();
    server.join();
    session::SessionManager::destroy();
    imdb::Imdb::destroy();
    logger::Logger::destroy();
    return 0;
}