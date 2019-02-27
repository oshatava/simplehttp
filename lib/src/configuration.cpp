#include "configuration.h"
#include <regex>

using namespace server;

Configuration &Configuration::route(std::string method, std::string path, CreatorFn creatorFn){
    callbackCreatorMap[method][path]=creatorFn;
    return (*this);
}

Configuration &Configuration::error(int error, CreatorFn creatorFn){
    errorCreatorMap[error] = creatorFn;
    return (*this);
}

Configuration &Configuration::preProccessor(RequestPreProcessorFn preProccessor){
    preProccessors.push_back(preProccessor);
    return (*this);
}

Configuration &Configuration::postProccessor(ResponsePostProcessorFn postProccessor){
    postProccessors.push_back(postProccessor);
    return (*this);
}

Response *Configuration::createResponse(Request &request)
{
    std::string method = request.getMethod();
    std::string path = request.getPath();
    std::map<std::string, CreatorFn> methods = callbackCreatorMap[method];
    Response * ret = NULL;
    for (std::pair<std::string, CreatorFn> entry : methods)
    {
        std::regex rPath(entry.first);
        std::smatch match;
        if (std::regex_match(request.getPath(), match, rPath))
        {
            for (int i = 0; i < match.size(); i++)
            {
                std::stringstream index;
                index << "$" << i;
                request.getParamsPath()[index.str()] = match[i];
            }
            CreatorFn creatorFn = entry.second;
            if (creatorFn != NULL)
            {
                ret = creatorFn(request);
                break;
            }
        }
    }

    if(ret==NULL){
        CreatorFn errorPageCreatorFn = errorCreatorMap[RESPONSE_CODE_ERROR_404];
        if (errorPageCreatorFn != NULL)
        {
            ret = errorPageCreatorFn(request);
        }else{
            ret = new Err404Response(request);
        }
    }

    for(auto requestProccesor:this->preProccessors){
        requestProccesor(request);
    }

    ret->proccess();

    for(auto responseProccesor:this->postProccessors){
        responseProccesor(ret);
    }

    return ret;
}
