#include "logger.h"
#include "server.h"
#include "domain.h"
#include "configuration.h"

int main(){
    logger::Logger::create(logger::DEBUG);

    // Init http callbacks
    server::Configuration configuration(8081, 10);

    configuration
        .route(METHOD_GET, "/version", server::createT<server::VersionResponse>)
        .route(METHOD_GET, "/devices", server::createT<server::DevicesResponse>)
        .route(METHOD_GET, "/device/([0-9]+)", server::createT<server::DeviceResponse>)
        .error(RESPONSE_CODE_ERROR_404, server::createT<server::My404ErrorPage>)
        .preProccessor([](server::Request& request){

            logger::Logger::d() << "Method is " << request.getMethod().c_str() << logger::endl;
            logger::Logger::d() << "Path is " << request.getPath().c_str() << logger::endl;

            for(auto header:request.getHeaders()){
                logger::Logger::d() << "Header [" << header.first.c_str() << "] = " << header.second.c_str() << logger::endl;
            }
            for(auto parameter:request.getParamsGet()){
                logger::Logger::d() << "Get [" << parameter.first.c_str() << "] = " << parameter.second.c_str() << logger::endl;
            }
            for(auto parameter:request.getParamsPath()){
                logger::Logger::d() << "Path [" << parameter.first.c_str() << "] = " << parameter.second.c_str() << logger::endl;
            }
            for(auto parameter:request.getParamsPost()){
                logger::Logger::d() << "Post [" << parameter.first.c_str() << "] = " << parameter.second.c_str() << logger::endl;
            }
        })
        .postProccessor([](server::Response *response){
            response->getHeaders()["Server"] = SERVER_NAME;
        })
        .postProccessor([](server::Response *response){
            int retCode = response->getRetCode();
            logger::Logger::d() << "Response ret code is - " << retCode << logger::endl;
            for(auto header:response->getHeaders()){
                logger::Logger::d() << "Header [" << header.first.c_str() << "] = " << header.second.c_str() << logger::endl;
            }
        })
        
        ;

    // Init server
    server::Server server(configuration);
    
    // Start server
    server.start();
    server.join();
    logger::Logger::destroy();
    return 0;
}