#include "session.h"
#include "logger.h"
#include <ctime>

static session::SessionManager *sessionManager = NULL;

void session::SessionManager::create()
{
    if(sessionManager==NULL){
        sessionManager = new SessionManager();
    }
}

void session::SessionManager::destroy()
{
    if(sessionManager!=NULL){
        delete sessionManager;
    }
}
inline std::string charToHex(char ch){
    unsigned char uch = ch;
    char buffer[3];
    bzero(buffer, 3);
    sprintf(buffer, "%X", uch);
    return std::string(buffer);
}

std::string session::SessionManager::generateSessionKey(){
    std::time_t now = time(0);
    struct tm *timeinfo;
    char buffer[80];
    timeinfo = localtime(&now);
    strftime(buffer, 80, "%F %T", timeinfo);
    
    std::stringstream newKeySS;
    newKeySS<<buffer<<" "<<sessions.size()+1;
    std::string newKeyS = newKeySS.str();
    std::stringstream newKey;
    for(int i=0; i<newKeyS.length(); i++){
        newKey<<charToHex(newKeyS[i]);
    }

    return newKey.str();
}

server::Session session::SessionManager::getSession(std::string key)
{
    if(sessions.find(key)!=sessions.end()){
        return sessions[key];
    }
    return server::Session();
}

void session::SessionManager::mergeSession(std::string key, server::Session session)
{
    sessions[key] = session;
}

void session::SessionProvider(server::Request &r)
{
    if(sessionManager == NULL){   
        logger::Logger::e()<<"Session manager does not instanced!"<<logger::endl;     
        return;
    }

    std::string cookie = r.getHeaders()["Cookie"];
    r.setSession(sessionManager->getSession(cookie));
}

void session::SessionMerge(server::Response &r)
{
    if(sessionManager == NULL){   
        logger::Logger::e()<<"Session manager does not instanced!"<<logger::endl;     
        return;
    }
    std::string cookie = r.getRequest().getHeaders()["Cookie"];
    if(!cookie.empty()){        
        logger::Logger::d()<<"Cookie exists - "<< cookie << logger::endl;        
        sessionManager->mergeSession(cookie, r.getRequest().getSession());
    }else{
        std::string  key = sessionManager->generateSessionKey();        
        r.setHeader("Set-Cookie", key);
        logger::Logger::d()<<"Cookie does not exists. Generate new key - "<< key << logger::endl;
        sessionManager->mergeSession(key, r.getRequest().getSession());
    }
}
