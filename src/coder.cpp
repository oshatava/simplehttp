#include "coder.h"


void security::RequestDeCoder(server::Request &r){
    std::string enc = r.getHeaders()["Encrypted"];    
    if(enc == "1"){
        CaesarCoder coder;
        r.setBody(coder.decode(r.getBody()));
    }
}

void security::ResponseEnCoder(server::Response &r){
    std::string enc = r.getRequest().getHeaders()["Encrypted"];    
    if(enc == "1"){
        CaesarCoder coder;        
        r.setBody(coder.encode(r.getBody()));
    }
}

std::string security::CaesarCoder::encode(std::string source){
    for(int i = 0; i<source.length(); i++){
        source[i] = source[i]+1;
    }
    return source;
}

std::string security::CaesarCoder::decode(std::string source){
    for(int i = 0; i<source.length(); i++){
        source[i] = source[i]-1;
    }
    return source;
}
