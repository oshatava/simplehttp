#ifndef DOMAIN_H
#define DOMAIN_H
#include <map>
#include <iostream>
#include <string>
#include <sstream>
#include "entity.h"
#include "json.h"
#include "configuration.h"

namespace domain
{

server::Response VersionResponse(server::Request &request);
server::Response DevicesResponse(server::Request &request);
server::Response DeviceResponse(server::Request &request);
server::Response My404ErrorPage(server::Request &request);

class Device : public json::JSONObject
{
public:
  const std::string id;
  const int channelCount;

  Device(std::string id, int channelCount) : id(id), channelCount(channelCount)
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

} // namespace domain

namespace http_logger
{
void logResponse(server::Response &response);
void logRequest(server::Request &request);
} // namespace http_logger

#endif