#ifndef TASK_H
#define TASK_H
#include <thread>

namespace common
{

class Task
{
  private:
    std::thread *worker;
    volatile bool shouldStop;
    volatile bool stopped;    
    virtual void threadFuncInternal();
  public:
    Task();
    ~Task();
    virtual void start();
    virtual void stop();
    virtual void join();

    bool isStoped(){ return stopped; }
  protected:
    bool isShouldStop(){ return shouldStop; }
    virtual void threadFunc() = 0;
};

} // namespace tasks
#endif