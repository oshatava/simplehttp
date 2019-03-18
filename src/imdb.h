#ifndef IMDB_H
#define IMDB_H
#include "configuration.h"
#include "json.h"

#include <string.h>
#include <map>
#include <mutex>

namespace imdb
{

template <class K, class V>
class DataContainer
{
private:
  std::map<K, V> data;
  std::mutex writeMutex;

public:
  DataContainer() {}
  virtual ~DataContainer() {}

  std::vector<K> getIds()
  {
    std::vector<K> ret;
    for (auto p : data)
    {
      ret.push_back(p.first);
    }
    return ret;
  }
  
  std::vector<V> getItems(){
    std::vector<V> ret;
    for (auto p : data)
    {
      ret.push_back(p.second);
    }
    return ret;
  }

  V getItem(K id)
  {
    if(data.find(id)!=data.end()){
      return data[id];
    }else{
      return V();
    }
  }

  void setItem(K id, V value)
  {
    std::lock_guard<std::mutex> lg(writeMutex);
    data[id] = value;
  }
};

class DBDataInterface
{
public:
  DBDataInterface() {}
  virtual ~DBDataInterface() {}

  virtual std::vector<std::string> getIds() = 0;
  virtual std::vector<std::string> getItems(std::string filter) = 0;
  virtual std::string getItem(std::string id) = 0;
  virtual void setItem(std::string id, std::string value) = 0;
};

class DBRestInterface
{
public:
  DBRestInterface() {}
  virtual ~DBRestInterface() {}

  virtual server::Response getIds(server::Request &r) = 0;
  virtual server::Response getItem(server::Request &r) = 0;
  virtual server::Response setItem(server::Request &r) = 0;
};

template <class T, std::string (*T2S)(T &), T (*S2T)(std::string &), class Matcher>
class DataSource : public DBDataInterface
{
public:
  DataSource(DataContainer<std::string, T> &data) : data(data) {}
  virtual ~DataSource() {}
  virtual std::vector<std::string> getIds() { return data.getIds(); }
  virtual std::vector<std::string> getItems(std::string filter){
    std::vector<std::string> out;
    Matcher matcher(filter);    
    for(auto p:data.getItems()){
      if(matcher.match(p)){
        out.push_back(D2S(p));
      }
    }
    return out;
  }
  virtual std::string getItem(std::string id)
  {    
    T item = data.getItem(id);
    return T2S(item);
  };
  virtual void setItem(std::string id, std::string value) { data.setItem(id, S2T(value)); };

private:
  DataContainer<std::string, T> &data;
};

class MemoryDB : public DBRestInterface
{
private:
  DBDataInterface &data;

public:
  MemoryDB(DBDataInterface &data) : data(data) {}
  virtual ~MemoryDB() {}
  server::Response setItem(server::Request &r);
  server::Response getItem(server::Request &r);
  server::Response getIds(server::Request &r);
};

class Imdb
{
private:
  std::map<std::string, DBRestInterface *> endPoints;
  Imdb() {}
  ~Imdb() {}
  DBRestInterface *findDBIntreface(server::Request &r);

public:
  static void create();
  static void destroy();
  static void registerInterface(std::string apiPrefix, DBRestInterface *db);
  static server::Response getIds(server::Request &r);
  static server::Response getItem(server::Request &r);
  static server::Response setItem(server::Request &r);
};

void setUpHttpCallbacks(server::Configuration &configuration, std::string apiPrefix, DBRestInterface *db);

} // namespace imdb

#endif