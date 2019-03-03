#include "client.h"
#include "logger.h"
#include "task.h"
#include "configuration.h"
#include <sstream>
#include <thread>
#include <chrono>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace server;
using namespace logger;
using namespace common;

void Client::threadFunc()
{
    Logger::d() << thread << " Start client task for socket - " << clientSocketFD << endl;
    unsigned char buffer[256];
    std::vector<unsigned char> input;

    while (!isShouldStop())
    {
        bzero(buffer, 256);
        int n = read(clientSocketFD, buffer, 255);
        if (n < 0)
        {
            Logger::e() << "ERROR reading from socket" << logger::endl;
            break;
        }  
        
        if(n>0){
            input.insert(input.end(), buffer, buffer+n);      
        }

        if (n < 255)
        {
            break;
        }
    }

    Response response = configuration.createResponse(input.data(), input.size());
    std::string out = response.build();

    send(clientSocketFD, out.c_str(), out.length(), 0);

    close(clientSocketFD);
    Logger::d() << "Stop client task and close socket - " << clientSocketFD << endl;    
}

Client::Client(int clientSocketFD, Configuration& configuration) : Task(), configuration(configuration)
{
    this->clientSocketFD = clientSocketFD;
    Logger::d() << thread << " Create client task" << logger::endl;
}

Client::~Client()
{
    Logger::d() << "Delete client - " << clientSocketFD << endl;    
    stop();
}