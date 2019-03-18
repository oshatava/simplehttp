#include "http.h"
#include "logger.h"
#include "utils.h"
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>
#include <regex>
#include <map>
#include <string>
#include "utils.h"

using namespace server::utils;

inline void parseParameters(std::map<std::string, std::string> &parameters, std::string line)
{
    std::istringstream iss(line);
    std::string s;
    while (getline(iss, s, '&'))
    {
        std::istringstream _iss(s);
        std::string _s;
        std::string name = "";
        std::string value = "";
        int index = 0;
        while (getline(_iss, _s, '='))
        {
            if (index == 0)
            {
                name = _s;
            }
            else if (index == 1)
            {
                value = _s;
                break;
            }
            index++;
        }
        trim(name);
        trim(value);
        parameters[name] = urlDecode(value);
    }
}

inline std::string parsePathParams(std::string line, std::map<std::string, std::string> &parameters)
{
    if(line.find("?")==std::string::npos){
        return line;
    }

    std::istringstream iss(line);
    std::string s;  
    std::string path;    
    int index = 0;
    while (getline(iss, s, '?'))
    {
        if (index == 0)
        {
            path = s;
        }else{
            parseParameters(parameters, s);
        }
        index++;
    }
    return path;
}

inline std::string parsePath(std::string line, std::map<std::string, std::string> &parameters)
{
    std::istringstream iss(line);
    std::string s;
    int index = 0;
    while (getline(iss, s, ' '))
    {
        if (index == 1)
        {        
            return parsePathParams(s, parameters);
        }
        index++;
    }
    return "/";
}

inline void parseHeader(std::map<std::string, std::string> &headers, std::string line)
{
    std::regex rLine("([a-zA-Z0-9-]+): (.*)");
    std::smatch match;
    std::string name = "";
    std::string value = "";
    if(std::regex_match(line, match, rLine)){            
        if(match.size()==3){
            name = match[1];
            value = match[2];
            trim(name);
            trim(value);
            headers[name] = value;
        }
    }
}

inline void parseHttpRequest(std::string requestRaw, server::Request &r)
{
    std::stringstream data;
    std::istringstream f(requestRaw);
    std::string line;
    int index = 0;
    while (std::getline(f, line))
    {
        trim(line);
        if (index == 0)
        {
            if (line.rfind(METHOD_GET, 0) == 0)
            {
                r.setMethod(METHOD_GET);
            }
            else if (line.rfind(METHOD_PUT, 0) == 0)
            {
                r.setMethod(METHOD_PUT);
            }
            else if (line.rfind(METHOD_POST, 0) == 0)
            {
                r.setMethod(METHOD_POST);
            }
            else if (line.rfind(METHOD_DELETE, 0) == 0)
            {
                r.setMethod(METHOD_DELETE);
            }
            r.setPath(parsePath(line, r.getParamsGet()));
            index++;
        }
        else if (index == 1)
        {
            if (line.empty())
            {
                index++;
            }
            else
            {
                parseHeader(r.getHeaders(), line);
            }
        }
        else if (index == 2)
        {
            data << line << std::endl;
        }
    }
    std::string body = data.str();
    trim(body);
    r.setBody(body);
    parseParameters(r.getParamsPost(), body);
}

server::Request http::requestProvider(const std::vector<char> &data){
    server::Request ret;
    int size = data.size();
    logger::Logger::d() << "Input data size "<<size<<logger::endl;
    if(size>0){
        std::string datas(data.begin(), data.end());
        logger::Logger::d() << "Input data: " << logger::endl<< datas << logger::endl;
        parseHttpRequest(datas, ret);
    }
    return ret;
}

