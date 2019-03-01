#ifndef RESPONSE_H
#define RESPONSE_H
#include <map>
#include <iostream>
#include <string>
#include <sstream>

namespace server
{
#define FIRMWARE_VERSION "0.0.1"
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
  std::string body;
  std::map<std::string, std::string> headers;
  std::map<std::string, std::string> paramsPost;
  std::map<std::string, std::string> paramsGet;
  std::map<std::string, std::string> paramsPath;

public:
  Request()
  {
  }

  Request(std::string requestRaw);

  Request(std::string path,
          std::string method,
          std::string body,
          std::map<std::string, std::string> headers,
          std::map<std::string, std::string> paramsPost,
          std::map<std::string, std::string> paramsGet,
          std::map<std::string, std::string> paramsPath)
  {
    this->path = path;
    this->method = method;
    this->body = body;
    this->headers = headers;
    this->paramsPost = paramsPost;
    this->paramsGet = paramsGet;
    this->paramsPath = paramsPath;
  }

  Request(const Request &r) : Request(r.path, r.method, r.body, r.headers, r.paramsPost, r.paramsGet, r.paramsPath)
  {
  }

  std::string getMethod() { return method; }
  std::string getPath() { return path; }
  std::string getBody() { return body; }
  std::map<std::string, std::string> &getHeaders() { return headers; }
  std::map<std::string, std::string> &getParamsGet() { return paramsGet; }
  std::map<std::string, std::string> &getParamsPost() { return paramsPost; }
  std::map<std::string, std::string> &getParamsPath() { return paramsPath; }

  void setMethod(std::string method) { this->method = method; }
  void setPath(std::string path) { this->path = path; }
  void setBody(std::string body) { this->body = body; }
  
};

class Response
{
protected:
  Request &request;
  std::map<std::string, std::string> headers;
  std::stringstream body;
  int retCode;

public:
  Response(Request &request,
           std::map<std::string, std::string> headers,
           std::string body,
           int retCode) : request(request), headers(headers), retCode(retCode)
  {
    this->body << body;
  }

  Response(Request &request) : request(request)
  {
  }

  Response(const Response &response) : Response(response.request, response.headers, response.body.str(), response.retCode)
  {
  }

  virtual ~Response() {}
  std::string build();

  // Getters
  std::map<std::string, std::string> &getHeaders() { return headers; }
  std::string getBody() { return body.str(); }
  int getRetCode() { return retCode; }
  Request getRequest(){return request;}
  // Builder
  Response &setHeaders(std::map<std::string, std::string> headers)
  {
    this->headers.clear();
    this->headers = headers;
    return (*this);
  }

  Response &setHeader(std::string name, std::string value)
  {
    this->headers[name] = value;
    return (*this);
  }

  Response &setBody(std::string body)
  {
    this->body.str(std::string());
    this->body << body;
    return (*this);
  }

  Response &appendBody(std::string body)
  {
    this->body << body;
    return (*this);
  }

  Response &setRetCode(int code)
  {
    this->retCode = code;
    return (*this);
  }
};

} // namespace server
#endif