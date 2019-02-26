#include "json.h"
#include <iostream>
#include <ctime>

using namespace json;

JSON::JSON(){    
    hasValueAdded = false;
    parent = NULL;
    this->endStr = "}";
    stream()<<"{";
}

std::stringstream &JSON::stream()
{
    return this->stringstream;
}