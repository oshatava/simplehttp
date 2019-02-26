#ifndef JSON_H
#define JSON_H
#include <string>
#include <sstream>
#include <thread>
#include <vector>
#include "logger.h"

namespace json
{

class JSON
{
  private:
    std::stringstream stringstream;
    std::stringstream& stream();
    bool hasValueAdded;
    std::string endStr;
    JSON* parent;
    std::vector<JSON*> children;

    JSON *newChild(std::string startStr, std::string endStr){
        JSON * child = new JSON(this, startStr, endStr);
        children.push_back(child); 
        return child;
        }

  protected:
    virtual JSON &newObject(){return *newChild("{", "}");}
    virtual JSON &newArray(){return *newChild("[", "]");}
    virtual std::stringstream &putName(std::string &name){stream()<<(hasValueAdded?",":"")<<"\""<<name<<"\":"; hasValueAdded = true; return stream();}
    virtual std::stringstream &putName(const char *name){std::string _name(name); return putName(_name);}

    virtual std::stringstream &putQuotedValue(std::string &value){stream()<<"\""<<value<<"\""; return stream();}
    virtual std::stringstream &putQuotedValue(const char *value){stream()<<"\""<<value<<"\""; return stream();}

  public:    
    JSON();
    JSON(JSON *parent, std::string startStr, std::string endStr){this->parent = parent; this->endStr = endStr; stream()<<startStr; hasValueAdded = false;}
    JSON(std::string startStr, std::string endStr){this->endStr = endStr; stream()<<startStr; hasValueAdded = false;}
    ~JSON(){
        for(JSON* child:children){
            delete child;            
        }
        children.clear();
    }

    JSON &end(){
        if(parent == NULL){
            stream()<<endStr;
            return (*this);
        }
        std::string str = stream().str();
        (*parent).stream()<<str<<endStr;
        return (*parent);
    }

    std::string str(){ return stream().str();}

    template <class T, class V> JSON &value(T name, V val){putName(name); putQuotedValue(val); return(*this);}
    template <class T> JSON &value(T name, int val){putName(name)<<val; return(*this);}
    template <class T> JSON &value(T name, long val){putName(name)<<val; return(*this);}
    template <class T> JSON &value(T name, float val){putName(name)<<val; return(*this);}
    template <class T> JSON &value(T name, bool val){putName(name)<<(val?"true":"false"); return(*this);}
    template <class T> JSON &value(T name, double val){putName(name)<<val; return(*this);}
    template <class T> JSON &valueNull(T name){putName(name)<<"null"; return(*this);}
    
    JSON &object(){stream()<<(hasValueAdded?",":""); return newObject();}
    JSON &object(const char* name){putName(name); return newObject();}    
    JSON &object(std::string &name){putName(name); return newObject();}    
    template <class N, class T> JSON &object(N name, T& t, JSON &(*objectPacker)(JSON &, T&)){putName(name); return objectPacker(newObject(), t);}
    template <class T> JSON &object(T& t, JSON &(*objectPacker)(JSON &, T&)){stream()<<(hasValueAdded?",":""); return objectPacker(newObject(), t);}
    template <class T> JSON &object(T& t){stream()<<(hasValueAdded?",":""); return t.pack(newObject());}

    template <class T> JSON &array(T name){putName(name); return newArray();}
    template <class N, class T> JSON &array(N name, std::vector<T> objects, JSON &(*objectPacker)(JSON &, T&)){
        putName(name); 
        hasValueAdded = true;
        return newArray().objects(objects, objectPacker).end();
    }

    template <class T> JSON &objects(std::vector<T> objects, JSON &(*objectPacker)(JSON &, T&)){
        for(T t:objects){
            object(t, objectPacker);
        }
        return (*this);
    }
};

class JSONObject{
    public:
    virtual json::JSON &pack(json::JSON &json) = 0; 
};

} // namespace json

#endif