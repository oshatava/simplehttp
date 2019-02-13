#ifndef RESPONSE_FABRIC_H
#define RESPONSE_FABRIC_H
#include <map>
#include <iostream>
#include <string>
#include <sstream>
#include "response.h"

namespace server
{

typedef Response* (*CreatorFn)(Request& request);

template<typename T> Response * createT(Request& request) { return new T(request); }

class ResponseFabric
{
  public:
    virtual ~ResponseFabric(){}
    void addRoute(std::string method, std::string path, CreatorFn creatorFn);
    Response* createResponse(Request &request);
    static ResponseFabric* get();
  private:    
    std::map<std::string, std::map<std::string, CreatorFn>> callbackCreatorMap;
    ResponseFabric() {}
};

class Err404Response:public Response{
    public:
    Err404Response(Request &request):Response(request){}
    virtual int on(std::stringstream &out){
      headers[HEADER_CONTENT_TYPE] = CONTENT_TYPE_TEXT;
      out<<"Page not found";
      return 404;
    }
};

} // namespace server
#endif