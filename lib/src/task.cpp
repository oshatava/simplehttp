#include "task.h"
#include "logger.h"
#include <sstream>
#include <thread>
#include <chrono>

using namespace common;
using namespace logger;

void Task::start()
{
    stop();
    shouldStop = false;
    stopped = false;
    worker = new std::thread(&Task::threadFuncInternal, this);
}

void Task::threadFuncInternal()
{
    try{
        threadFunc();
    }catch(const std::exception& e){
        Logger::e()<<e.what()<<logger::endl;
    }
    stopped = true;
}

void Task::stop()
{
    if (worker != NULL)
    {
        shouldStop = true;
        join();
        delete worker;
        worker = NULL;
    }
}

void Task::join()
{
    if (worker != NULL)
    {
        if (worker->joinable())
        {
            worker->join();
        }
    }
    stopped = true;
}

Task::Task()
{
    this->worker = NULL;
    stopped = true;
}

Task::~Task()
{
    stop();
}