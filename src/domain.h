#ifndef DOMAIN_H
#define DOMAIN_H
#include <map>
#include <iostream>
#include <string>
#include <sstream>
#include "entity.h"
#include "json.h"
#include "configuration.h"
#include "imdb.h"
#include "utils.h"

namespace domain
{


class Device
{
public:
  std::string id;
  int channelCount;

public:
  Device(std::string id, int channelCount) : id(id), channelCount(channelCount)
  {
    std::stringstream ss;
    ss << channelCount;
  }
  Device(const Device &d) : Device(d.id, d.channelCount) {}
  Device() : Device("no_id", 0) {}
};

server::Response VersionResponse(server::Request &request);
server::Response My404ErrorPage(server::Request &request);

std::string D2S(Device &device);
Device S2D(std::string &str);

class DeviceMatcher:public server::utils::Matcher<domain::Device>{
  public:
    DeviceMatcher(std::string &term):Matcher<Device>(){
      matcher(domain::Device, id);
      matcher(domain::Device, channelCount);
      build(term);
    }
};


} // namespace domain

namespace http_logger
{
void logResponse(server::Response &response);
void logRequest(server::Request &request);
} // namespace http_logger

#endif