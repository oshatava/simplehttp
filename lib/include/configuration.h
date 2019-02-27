#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include "response.h"
#include "html.h"

namespace server
{

typedef Response* (*CreatorFn)(Request& request);
typedef void (*RequestPreProcessorFn)(Request& request);
typedef void (*ResponsePostProcessorFn)(Response* request);

template<typename T> Response * createT(Request& request) { return new T(request); }

class Configuration
{
  public:
    virtual ~Configuration(){}
    Configuration &route(std::string method, std::string path, CreatorFn creatorFn);
    Configuration &error(int error, CreatorFn creatorFn);
    Configuration &preProccessor(RequestPreProcessorFn preProccessor);
    Configuration &postProccessor(ResponsePostProcessorFn postProccessor);
    Response* createResponse(Request &request);
    Configuration(int port, int maxConnection){
      this->port = port;
      this->maxConnection = maxConnection;
    }
    unsigned int getPort(){return port;}
    unsigned int getMaxConnection(){return maxConnection;}

  private:    
    unsigned int port;
    unsigned int maxConnection;
    std::map<std::string, std::map<std::string, CreatorFn>> callbackCreatorMap;
    std::map<int, CreatorFn> errorCreatorMap;    
    std::vector<RequestPreProcessorFn> preProccessors;
    std::vector<ResponsePostProcessorFn> postProccessors;
};

class Err404Response:public Response{
    public:
    Err404Response(Request &request):Response(request){}
    virtual int on(std::stringstream &out){
      headers[HEADER_CONTENT_TYPE] = CONTENT_TYPE_TEXT;      
      out<<html::HTML()
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
      .build();
      return RESPONSE_CODE_ERROR_404;
    }
};

} // namespace server
#endif