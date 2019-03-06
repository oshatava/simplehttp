#ifndef HTML_H
#define HTML_H
#include <string>
#include <sstream>
#include <thread>
#include <vector>
#include "logger.h"

namespace html
{

class HTML
{
  private:
    std::stringstream bodyV;
    std::stringstream params;
    bool hasParamAdded;
    bool hasBodyAdded;
    std::string tagName;
    HTML* parent;
    std::vector<HTML*> children;

    HTML *newChild(std::string tagName){
        HTML * child = new HTML(this, tagName);
        children.push_back(child); 
        return child;
    }

    std::string openTag(){
        std::stringstream openTagS;
        openTagS<<"<"<<tagName;
        if(hasParamAdded)
            openTagS<<" "<< params.str();
        if(!hasBodyAdded)
            openTagS<<"/";
        openTagS<<">";
        return openTagS.str();
    }

    std::string closeTag(){
        if(!hasBodyAdded){
            return "";
        }            
        std::stringstream closeTagS;
        closeTagS<<"</"<<tagName<<">";
        return closeTagS.str();
    }

  protected:

    template<class N, class V> void putParam(N name, V value){        
        params<<(hasParamAdded?" ":"")<<name<<"=\""<<value<<"\"";
        hasParamAdded = true;
    }

  public:    
    HTML(HTML *parent, std::string tagName){this->parent = parent; this->tagName = tagName; hasParamAdded = false; hasBodyAdded = false;}
    HTML(std::string tagName):HTML(NULL, tagName){}
    HTML():HTML(NULL, "html"){}
    ~HTML(){
        for(HTML* child:children){
            delete child;            
        }
        children.clear();
    }

    HTML &end(){
        if(parent == NULL){            
            return (*this);
        }        
        parent->bodyV<<build();
        return (*parent);
    }

    std::string build(){ 
        std::stringstream stream;
        stream<<((parent==NULL)?"<!DOCTYPE html>":"")<<openTag()<<bodyV.str()<<closeTag();
        return stream.str();
    }

    template<class T> HTML &tag(T name){hasBodyAdded = true; return *newChild(name);}
    template<class N, class V> HTML &param(N name, V value){putParam(name, value); return(*this);}
    template<class V> HTML &body(V value){hasBodyAdded = true; bodyV<<value; return(*this);}

};

} // namespace json

#endif