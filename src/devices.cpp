#include "domain.h"
#include "json.h"


inline json::JSON &packDevice(json::JSON &json, domain::Device &device)
{
  return json
      .value("id", device.id.c_str())
      .value("channelCount", device.channelCount)
      .end();
}

std::string domain::D2S(domain::Device &device)
{
  json::JSON jsn;
  return packDevice(jsn, device).str();
}

domain::Device domain::S2D(std::string &str)
{
  // todo
  return domain::Device(str, 0);
}