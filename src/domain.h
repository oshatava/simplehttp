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

class DeviceResponse : public Response
{
  public:
    DeviceResponse(Request &request) : Response(request) {}
    virtual int on(std::stringstream &out);
};

class My404ErrorPage : public Response
{
  public:
    My404ErrorPage(Request &request) : Response(request) {}
    virtual int on(std::stringstream &out);
};

class Device:public json::JSONObject
{
  public:
    const std::string id;
    const int channelCount;

    Device(std::string id, int channelCount):id(id),channelCount(channelCount)
    {
    }

    virtual json::JSON &pack(json::JSON &json)
    {
        return json
            .value("id", id)
            .value("channelCount", channelCount)
            .end();
    }
};

inline json::JSON &packDevice(json::JSON &json, Device &device)
{
    return json
        .value("id", device.id.c_str())
        .value("channelCount", device.channelCount)
        .end();
}

} // namespace server

#endif