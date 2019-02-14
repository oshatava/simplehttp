#ifndef DOMAIN_H
#define DOMAIN_H
#include <map>
#include <iostream>
#include <string>
#include <sstream>
#include "response.h"
#include "json.h"

namespace server
{

class VersionResponse : public Response
{
  public:
    VersionResponse(Request &request) : Response(request) {}
    virtual int on(std::stringstream &out);
};


class DevicesResponse : public Response
{
  public:
    DevicesResponse(Request &request) : Response(request) {}
    virtual int on(std::stringstream &out);
};

class Device{
    public:
        std::string id;
        int channelCount;
    public:
        Device(std::string id, int channelCount){
            this -> id = id;
            this -> channelCount = channelCount;
        }


};

inline json::JSON& packDevice(json::JSON& json, Device &device){
    return 
    json
        .value("id", device.id.c_str())
        .value("channelCount", device.channelCount)
    .end();
}


} // namespace server

#endif