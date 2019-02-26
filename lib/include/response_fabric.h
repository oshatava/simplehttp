#ifndef RESPONSE_FABRIC_H
#define RESPONSE_FABRIC_H
#include <map>
#include <iostream>
#include <string>
#include <sstream>
#include "response.h"
#include "html.h"

namespace server
{

typedef Response* (*CreatorFn)(Request& request);

template<typename T> Response * createT(Request& request) { return new T(request); }

class ResponseFabric
{
  public:
    virtual ~ResponseFabric(){}
    void addRoute(std::string method, std::string path, CreatorFn creatorFn);
    void setErrorResponse(int error, CreatorFn creatorFn);
    Response* createResponse(Request &request);
    static ResponseFabric* get();
  private:    
    std::map<std::string, std::map<std::string, CreatorFn>> callbackCreatorMap;
    std::map<int, CreatorFn> errorCreatorMap;
    ResponseFabric() {}
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