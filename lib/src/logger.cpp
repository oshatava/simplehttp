#include "logger.h"
#include <iostream>
#include <ctime>

using namespace logger;

static Logger *INSTANCE = NULL;

Logger *Logger::get()
{
    if (INSTANCE == NULL)
    {
        INSTANCE = new Logger(DEBUG);
    }
    return INSTANCE;
}

Logger &Logger::d()
{
    return get()->start(DEBUG, "D: ");
}

Logger &Logger::e()
{
    return get()->start(PRODUCTION, "E: ");
}

Logger &Logger::w()
{
    return get()->start(PRODUCTION, "W: ");
}

Logger &Logger::i()
{
    return get()->start(PRODUCTION, "I: ");
}

Logger &Logger::start(LogLevel logLevel, std::string prefix)
{
    std::time_t now = time(0);
    struct tm *timeinfo;
    char buffer[80];

    this->logLevelForChain = logLevel;

    timeinfo = localtime(&now);
    strftime(buffer, 80, "%F %T", timeinfo);

    stream() << buffer << ":" << prefix;
    return *this;
}

void Logger::endl()
{
    stream() << std::endl;
}

void Logger::flush()
{
    stream().flush();
}

void Logger::thread()
{
    std::thread::id this_id = std::this_thread::get_id();
    stream() << this_id;
}

Logger &Logger::operator<<(long &val)
{
    stream() << val;
    return (*this);
}

Logger &Logger::operator<<(unsigned long &val)
{
    stream() << val;
    return (*this);
}

Logger &Logger::operator<<(Logger &(*manupulator)(Logger &))
{
    return manupulator(*this);
}

Logger &Logger::operator<<(const char *val)
{
    stream() << val;
    return (*this);
}

Logger &Logger::operator<<(std::string &val)
{
    stream() << val;
    return (*this);
}

Logger &Logger::operator<<(int &val)
{
    stream() << val;
    return (*this);
}

Logger &Logger::operator<<(unsigned int &val)
{
    stream() << val;
    return (*this);
}

void Logger::create(LogLevel logLevel)
{
    INSTANCE = new Logger(logLevel);
}

void Logger::destroy()
{
    if (INSTANCE != NULL)
    {
        delete INSTANCE;
        INSTANCE = NULL;
    }
}

std::ostream &Logger::stream()
{
    return std::cout;
}

Logger::Logger(LogLevel logLevel)
{
    this->logLevel = logLevel;
}

Logger::~Logger()
{
}
