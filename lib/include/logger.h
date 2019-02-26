#ifndef LOGGER_H
#define LOGGER_H
#include <string>
#include <sstream>
#include <thread>

namespace logger
{

enum LogLevel
{
    DEBUG = 0,
    PRODUCTION = 1
};

class Logger
{
  private:
    static Logger *get();
    LogLevel logLevel;
    LogLevel logLevelForChain;
    bool isEmpty;
    Logger(LogLevel logLevel);
    ~Logger();
    Logger &start(LogLevel logLevel, std::string prefix);
    std::ostream& stream();

  public:
    static Logger &d();
    static Logger &e();
    static Logger &w();
    static Logger &i();

    static void create(LogLevel logLevel);
    static void destroy();

    void endl();
    void flush();
    void thread();

    Logger &operator<<(Logger &(*manupulator)(Logger &));
    Logger &operator<<(int &val);
    Logger &operator<<(unsigned int &val);
    Logger &operator<<(const char *val);
    Logger &operator<<(std::string &val);
    Logger &operator<<(long &val);
    Logger &operator<<(unsigned long &val);    
    Logger &operator<<(void *val);    
};

inline Logger &endl(Logger &logger)
{
    logger.endl();
    logger.flush();
    return logger;
}

inline Logger &thread(Logger &logger)
{
    logger.thread();
    return logger;
}

} // namespace logger

#endif