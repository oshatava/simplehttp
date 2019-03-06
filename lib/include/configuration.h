#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include "entity.h"
#include "html.h"

namespace server
{

typedef Request (*RequestProvider)(const std::vector<char> &data);
typedef Response (*ResponseProvider)(Request &request);
typedef void (*RequestProccesor)(Request &response);
typedef void (*ResponseProccesor)(Response &response);

class Configuration
{
  public:
    virtual ~Configuration(){}
    Configuration &route(std::string method, std::string path, ResponseProvider responseProvider);
    Configuration &error(int error, ResponseProvider responseProvider);
    Configuration &preProccessorRaw(RequestProvider preProccessorRaw);
    Configuration &preProccessor(RequestProccesor preProccessor);
    Configuration &postProccessor(ResponseProccesor postProccessor);
    Response createResponse(const std::vector<char> &data);
    Configuration(int port, int maxConnection);
    unsigned int getPort(){return port;}
    unsigned int getMaxConnection(){return maxConnection;}

  private:    
    unsigned int port;
    unsigned int maxConnection;
    std::map<std::string, std::map<std::string, ResponseProvider>> callbackCreatorMap;
    std::map<int, ResponseProvider> errorCreatorMap;    
    std::vector<RequestProccesor> preProccessors;
    std::vector<ResponseProccesor> postProccessors;
    RequestProvider preProccessorRawFunc;
};

} // namespace server
#endif