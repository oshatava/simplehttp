#ifndef SESSION_H
#define SESSION_H
#include "entity.h"
namespace session
{

class SessionManager
{
  public:
    static void create();
    static void destroy();
    std::string generateSessionKey();
    server::Session getSession(std::string key);
    void mergeSession(std::string key, server::Session session);
  private:
    std::map<std::string, server::Session> sessions;
};

void SessionProvider(server::Request &r);
void SessionMerge(server::Response &r);

} // namespace session

#endif