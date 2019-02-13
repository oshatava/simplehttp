#ifndef DOMAIN_H
#define DOMAIN_H
#include <map>
#include <iostream>
#include <string>
#include <sstream>
#include "response.h"

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


} // namespace server

#endif