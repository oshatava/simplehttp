#ifndef HTTP_H
#define HTTP_H
#include "entity.h"

namespace http
{
server::Request requestProvider(const std::vector<char> &data);
}
#endif