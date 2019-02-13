#ifndef CLIENT_H
#define CLIENT_H
#include <thread>
#include "task.h"

namespace server
{

class Client : public common::Task
{
private:
  int clientSocketFD;

public:
  Client(int clientSocketFD);
  virtual ~Client();

protected:
  virtual void threadFunc();
};

} // namespace server
#endif