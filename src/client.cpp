#include "client.h"
#include "logger.h"
#include "task.h"
#include "response_fabric.h"
#include <sstream>
#include <thread>
#include <chrono>

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
    char buffer[256];
    std::stringstream input;

    while (!isShouldStop())
    {
        bzero(buffer, 256);
        int n = read(clientSocketFD, buffer, 255);
        if (n < 0)
        {
            Logger::e() << "ERROR reading from socket" << logger::endl;
            break;
        }
        input << buffer;
        if (n < 255)
        {
            std::string data = input.str();
            Logger::d() << "Input data: " << logger::endl<< data << logger::endl;
            break;
        }
    }

    Request request(input.str());
    Response *response = ResponseFabric::get()->createResponse(request);
    std::string out = response->getResponse().str();
    delete response;
    
    send(clientSocketFD, out.c_str(), out.length(), 0);

    close(clientSocketFD);
    Logger::d() << "Stop client task and close socket - " << clientSocketFD << endl;    
}

Client::Client(int clientSocketFD) : Task()
{
    this->clientSocketFD = clientSocketFD;
    Logger::d() << thread << " Create client task" << logger::endl;
}

Client::~Client()
{
    Logger::d() << "Delete client - " << clientSocketFD << endl;    
    stop();
}