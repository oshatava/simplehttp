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
    JSON &start(std::string prefix);
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

  public:    
    JSON();
    JSON(JSON *parent, std::string startStr, std::string endStr){this->parent = parent; this->endStr = endStr; stream()<<startStr; hasValueAdded = false;}
    JSON(std::string startStr, std::string endStr){this->endStr = endStr; stream()<<startStr; hasValueAdded = false;}
    ~JSON(){
        logger::Logger::d()<<"JSON destroy"<<logger::endl;
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

    JSON &array(const char *name){putName(name); return newArray();}
    JSON &value(const char *name, const char *val){putName(name)<<"\""<<val<<"\""; return(*this);}
    JSON &value(const char *name, int val){putName(name)<<val; return(*this);}
    JSON &value(const char *name, long val){putName(name)<<val; return(*this);}
    JSON &value(const char *name, float val){putName(name)<<val; return(*this);}
    JSON &value(const char *name, bool val){putName(name)<<(val?"true":"false"); return(*this);}
    JSON &value(const char *name, double val){putName(name)<<val; return(*this);}
    JSON &valueNull(const char *name){stream()<<(hasValueAdded?",":"")<<"\""<<name<<"\": null"; return(*this);}
    JSON &value(std::string &name, std::string &val){putName(name)<<"\""<<val<<"\""; return(*this);}
    JSON &value(std::string &name, int val){putName(name)<<val; return(*this);}
    JSON &value(std::string &name, long val){putName(name)<<val; return(*this);}
    JSON &value(std::string &name, float val){putName(name)<<val; return(*this);}
    JSON &value(std::string &name, bool val){putName(name)<<(val?"true":"false"); return(*this);}
    JSON &value(std::string &name, double val){putName(name)<<val; return(*this);}
    JSON &valueNull(std::string &name){stream()<<(hasValueAdded?",":"")<<"\""<<name<<"\": null"; return(*this);}

    JSON &object(){stream()<<(hasValueAdded?",":""); return newObject();}
    JSON &object(const char *name){putName(name); return newObject();}
    JSON &object(std::string &name){putName(name); return newObject();}
    template <class T> JSON &object(const char *name, T& t, JSON &(*objectPacker)(JSON &, T&)){putName(name); return objectPacker(newObject(), t);}
    template <class T> JSON &object(T& t, JSON &(*objectPacker)(JSON &, T&)){stream()<<(hasValueAdded?",":""); return objectPacker(newObject(), t);}

    template <class T> JSON &array(const char *name, std::vector<T> objects, JSON &(*objectPacker)(JSON &, T&)){
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



} // namespace json

#endif