#include "server.h"
#include "logger.h"
#include "task.h"
#include "client.h"
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

void Server::threadFunc()
{
    int port = configuration.getPort();
    Logger::d() << thread << " Start server on port:"<< port << endl;

    int sockfd, newsockfd;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    std::vector<Client *> clients;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        Logger::e() << "ERROR opening socket" << logger::endl;
        return;
    }
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(configuration.getPort());
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        Logger::e() << "ERROR on binding" << logger::endl;
        return;
    }

    listen(sockfd, configuration.getMaxConnection());
    clilen = sizeof(cli_addr);

    while (!isShouldStop())
    {
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0)
        {
            Logger::e() << "ERROR on accept" << logger::endl;
            break;
        }
        int port = ((int)(cli_addr.sin_port));
        Logger::i() << "Got connection from port " << port << logger::endl;

        clients.erase(std::remove_if(clients.begin(), clients.end(), [](Client *c){
            if(c!=NULL && c->isStoped()){
                delete c;
            }  
            return (c != NULL) ? c->isStoped() : true;             
        }), clients.end());

        Client *client = new Client(newsockfd, this->configuration);
        client->start();
        clients.push_back(client);
    }

    for (Client * client:clients)
    {        
        delete client;
    }
    
    clients.clear();
    close(sockfd);
    Logger::d() << "Stop server" << endl;
}

Server::Server(Configuration &_configuration):Task(), configuration(_configuration)
{    
}

Server::~Server()
{
    stop();
}