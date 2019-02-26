#include "response_fabric.h"
#include <regex>

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
    for(std::pair<std::string, CreatorFn> entry:methods){        
        std::regex rPath(entry.first);
        std::smatch match;
        if(std::regex_match(request.getPath(), match, rPath)){            
            for(int i=0; i<match.size(); i++){
                std::stringstream index;
                index<<"$"<<i;
                request.getParamsPath()[index.str()] = match[i];
            }
            CreatorFn creatorFn = entry.second;
            if(creatorFn!=NULL){
                return creatorFn(request);
            }
        }
    }
   return new Err404Response(request);   
}
