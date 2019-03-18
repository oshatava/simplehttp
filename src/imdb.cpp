#include "imdb.h"
#include "json.h"

static imdb::Imdb *imdbInstance = NULL;

server::Response imdb::MemoryDB::setItem(server::Request &r)
{
    std::string key = r.getParamsPath()["$1"];
    std::string value = r.getBody();
    data.setItem(key, value);

    return server::Response(r)
        .setHeader(HEADER_CONTENT_TYPE, CONTENT_TYPE_JSON)
        .setBody(data.getItem(key))
        .setRetCode(RESPONSE_CODE_OK_200);
}

server::Response imdb::MemoryDB::getItem(server::Request &r)
{
    std::string key = r.getParamsPath()["$1"];
    std::string value = data.getItem(key);

    return server::Response(r)
        .setHeader(HEADER_CONTENT_TYPE, CONTENT_TYPE_JSON)
        .setBody(value)
        .setRetCode(RESPONSE_CODE_OK_200);
}

server::Response imdb::MemoryDB::getIds(server::Request &r)
{
    json::JSON jsn;
    std::string filterBy = r.getParamsGet()["filter"];    
    std::vector<std::string> items = data.getItems(filterBy);
    int size = items.size();
    jsn.value("itemsCount", size);
    json::JSON &jsna = jsn.array("items");
    for (const auto p : items)
    {
        jsna.objectRaw(p);
    }
    jsna.end();
    return server::Response(r)
        .setHeader(HEADER_CONTENT_TYPE, CONTENT_TYPE_JSON)
        .setBody(jsn.end().str())
        .setRetCode(RESPONSE_CODE_OK_200);
}

void imdb::Imdb::create()
{
    if (imdbInstance == NULL)
    {
        imdbInstance = new imdb::Imdb();
    }
}

void imdb::Imdb::destroy()
{
    if (imdbInstance != NULL)
    {
        delete imdbInstance;
    }
}

void imdb::Imdb::registerInterface(std::string apiPrefix, imdb::DBRestInterface *db)
{
    imdbInstance->endPoints[apiPrefix] = db;
}

imdb::DBRestInterface *imdb::Imdb::findDBIntreface(server::Request &r)
{
    std::string path = r.getPath();
    for (auto p : imdbInstance->endPoints)
    {
        std::string key = p.first;
        if (path.find(key) == 0)
        {
            return p.second;
        }
    }
    return NULL;
}

inline void assertInterfaceIsFound(server::Request &r, imdb::DBRestInterface *db){
    if (db == NULL)
    {
        server::ServerError::throwFor("DBRestInterface not found!", r);
    }
}

server::Response imdb::Imdb::getIds(server::Request &r)
{
    DBRestInterface *db = imdbInstance->findDBIntreface(r);
    assertInterfaceIsFound(r, db);
    return db->getIds(r);
}

server::Response imdb::Imdb::getItem(server::Request &r)
{
    DBRestInterface *db = imdbInstance->findDBIntreface(r);
    assertInterfaceIsFound(r, db);
    return db->getItem(r);
}

server::Response imdb::Imdb::setItem(server::Request &r)
{
    DBRestInterface *db = imdbInstance->findDBIntreface(r);
    assertInterfaceIsFound(r, db);
    return db->setItem(r);
}

void imdb::setUpHttpCallbacks(server::Configuration &configuration, std::string apiPrefix, DBRestInterface *db)
{
    imdbInstance->registerInterface(apiPrefix, db);
    configuration.route(METHOD_GET, apiPrefix + "([/]?)", &imdb::Imdb::getIds);
    configuration.route(METHOD_GET, apiPrefix + "/([0-9a-zA-Z]+)", &imdb::Imdb::getItem);
    configuration.route(METHOD_PUT, apiPrefix + "/([0-9a-zA-Z]+)", &imdb::Imdb::setItem);
}
