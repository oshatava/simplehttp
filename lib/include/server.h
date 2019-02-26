#ifndef SERVER_H
#define SERVER_H
#include <thread>
#include "task.h"

namespace server
{

class Server : public common::Task
{
private:
  unsigned int port;
  unsigned int maxConnection;

public:
  Server(int port, int maxConnection);
  virtual ~Server();

protected:
  virtual void threadFunc();
};

} // namespace server
#endif