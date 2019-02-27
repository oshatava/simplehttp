#ifndef RESPONSE_H
#define RESPONSE_H
#include <map>
#include <iostream>
#include <string>
#include <sstream>

namespace server
{
#define FIRMWARE_VERSION  "0.0.1"
#define SERVER_NAME "OSH"

#define METHOD_GET "GET"
#define METHOD_POST "POST"
#define METHOD_PUT "PUT"
#define METHOD_DELETE "DELETE"

#define RESPONSE_CODE_ERROR_404 404
#define RESPONSE_CODE_OK_200 200


#define HEADER_CONTENT_TYPE "Content-Type"
#define CONTENT_TYPE_TEXT "text/html"
#define CONTENT_TYPE_JSON "application/json"

class Request
{
  private:
    std::string method;
    std::string path;
    std::string data;
    std::map<std::string, std::string> headers;
    std::map<std::string, std::string> paramsPost;
    std::map<std::string, std::string> paramsGet;
    std::map<std::string, std::string> paramsPath;

  public:
    Request(){

    }
    Request(std::string requestRaw);

    Request(std::string path,
            std::string method,
            std::map<std::string, std::string> headers,
            std::map<std::string, std::string> paramsPost,
            std::map<std::string, std::string> paramsGet,
            std::map<std::string, std::string> paramsPath)
    {
        this->path = path;
        this->method = method;
        this->headers = headers;
        this->paramsPost = paramsPost;
        this->paramsGet = paramsGet;
        this->paramsPath = paramsPath;        
    }
    std::string getMethod() { return method; }
    std::string getPath() { return path; }
    std::map<std::string, std::string> &getHeaders() { return headers; }
    std::map<std::string, std::string> &getParamsGet() { return paramsGet; }
    std::map<std::string, std::string> &getParamsPost() { return paramsPost; }
    std::map<std::string, std::string> &getParamsPath() { return paramsPath; }
};

class Response
{
  protected:
    Request& request;
    std::map<std::string, std::string> headers;
    std::stringstream body;
    std::stringstream stream;
    int retCode;
  public:    
    Response(Request& request):request(request)
    {
    }
    virtual ~Response(){}
    void proccess();
    std::stringstream &getResponse();
    virtual int on(std::stringstream &body) = 0;

    std::map<std::string, std::string> &getHeaders() { return headers; }
    std::string getBody() { return body.str(); }
    void setBody(std::string newBody) { 
      body.clear();
      body.flush();
      body<<newBody;
    }
    int getRetCode(){return retCode;}
    void setRetCode(int newRetCode){retCode = newRetCode;}
};

} // namespace server
#endif