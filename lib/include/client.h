#ifndef CLIENT_H
#define CLIENT_H
#include <thread>
#include "task.h"
#include "configuration.h"

namespace server
{

class Client : public common::Task
{
private:
  int clientSocketFD;
  Configuration& configuration;

public:
  Client(int clientSocketFD, Configuration& configuration);
  virtual ~Client();

protected:
  virtual void threadFunc();
};

} // namespace server
#endif