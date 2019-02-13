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

inline bool clientIsShouldBeRemoved(Client *c) 
{ 
    int res = ((c != NULL) ? c->isStoped() : true);
    Logger::d()<<"clientIsShouldBeRemoved - "<<res<<logger::endl;
    return (c != NULL) ? c->isStoped() : true; 
}

void Server::threadFunc()
{
    Logger::d() << thread << " Start server" << endl;

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
    serv_addr.sin_port = htons(this->port);
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        Logger::e() << "ERROR on binding" << logger::endl;
        return;
    }

    listen(sockfd, this->maxConnection);
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
        Logger::d() << "Got connection from port " << port << logger::endl;

        int was = clients.size();
        Logger::d() << "Clean up clients " << was << logger::endl;
        clients.erase(std::remove_if(clients.begin(), clients.end(), clientIsShouldBeRemoved), clients.end());

        Client *client = new Client(newsockfd);
        client->start();
        clients.push_back(client);
    }

    for (int i = 0; i < clients.size(); i++)
    {
        Client *client = clients[i];
        client->stop();
        delete client;
    }

    clients.clear();
    close(sockfd);
    Logger::d() << "Stop server" << endl;
}

Server::Server(int port, int maxConnection) : Task()
{
    this->port = port;
    this->maxConnection = maxConnection;
    unsigned long addr = (unsigned long)this;
    Logger::d() << thread << " Create server on port " << port << " for maximum clients " << maxConnection << " id " << addr << logger::endl;
}

Server::~Server()
{
    stop();
}