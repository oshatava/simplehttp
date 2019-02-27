#ifndef SERVER_H
#define SERVER_H
#include <thread>
#include "task.h"
#include "configuration.h"

namespace server
{

class Server : public common::Task
{
private:
  Configuration &configuration;

public:
  Server(Configuration &configuration);
  virtual ~Server();

protected:
  virtual void threadFunc();
};

} // namespace server
#endif