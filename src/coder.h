#ifndef CODER_H
#define CODER_H
#include "entity.h"
#include <string>

namespace security{

class Coder
{
public:
    virtual std::string encode(std::string source) = 0;
    virtual std::string decode(std::string source) = 0;
};

class CaesarCoder:public Coder{
public:    
    virtual std::string encode(std::string source);
    virtual std::string decode(std::string source);
};

void RequestDeCoder(server::Request &r);
void ResponseEnCoder(server::Response &r);

}


#endif