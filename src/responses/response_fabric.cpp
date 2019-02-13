#include "response_fabric.h"
using namespace server;

static ResponseFabric *INSTANCE = NULL;

ResponseFabric *ResponseFabric::get()
{
    if (INSTANCE == NULL)
    {
        INSTANCE = new ResponseFabric();
    }
    return INSTANCE;
}

void ResponseFabric::addRoute(std::string method, std::string path, CreatorFn creatorFn){
    callbackCreatorMap[method][path]=creatorFn;
}

Response *ResponseFabric::createResponse(Request &request){   
    std::string method = request.getMethod();
    std::string path = request.getPath();
    std::map<std::string, CreatorFn> methods = callbackCreatorMap[method];

    CreatorFn creatorFn = methods[path];
    if(creatorFn!=NULL){
        return creatorFn(request);
    }

   return new Err404Response(request);   
}
