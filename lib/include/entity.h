#ifndef ENTITY_H
#define ENTITY_H
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

#define RESPONSE_CODE_ERROR_500 500
#define RESPONSE_CODE_ERROR_404 404
#define RESPONSE_CODE_OK_200 200

#define HEADER_CONTENT_TYPE "Content-Type"
#define CONTENT_TYPE_TEXT "text/html"
#define CONTENT_TYPE_JSON "application/json"

class Session
{
private:
  std::map<std::string, std::string> data;
  bool dirty;

public:
  Session(const Session &s) : Session(s.data, s.dirty) {}
  Session(std::map<std::string, std::string> data, bool dirty) : data(data), dirty(dirty) {}
  Session() { dirty = false; }
  std::map<std::string, std::string> &getData() { return data; }
  void flush() { dirty = false; }
  void markAsDirty() { dirty = true; }
  bool isDirty() { return dirty; }
};

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
  Session session;

public:
  Request(std::string path,
          std::string method,
          std::string body,
          std::map<std::string, std::string> headers,
          std::map<std::string, std::string> paramsPost,
          std::map<std::string, std::string> paramsGet,
          std::map<std::string, std::string> paramsPath,
          Session session)
  {
    this->path = path;
    this->method = method;
    this->body = body;
    this->headers = headers;
    this->paramsPost = paramsPost;
    this->paramsGet = paramsGet;
    this->paramsPath = paramsPath;
    this->session = session;
  }

  Request(const Request &r) : Request(r.path, r.method, r.body, r.headers, r.paramsPost, r.paramsGet, r.paramsPath, r.session)
  {
  }

  Request() : Request("", "", "", std::map<std::string, std::string>(), std::map<std::string, std::string>(), std::map<std::string, std::string>(), std::map<std::string, std::string>(), Session())
  {
  }

  std::string getMethod() const { return method; }
  std::string getPath() const { return path; }
  std::string getBody() const { return body; }
  std::map<std::string, std::string> &getHeaders() { return headers; }
  std::map<std::string, std::string> &getParamsGet() { return paramsGet; }
  std::map<std::string, std::string> &getParamsPost() { return paramsPost; }
  std::map<std::string, std::string> &getParamsPath() { return paramsPath; }
  Session &getSession() { return session; }

  void setMethod(std::string method) { this->method = method; }
  void setPath(std::string path) { this->path = path; }
  void setBody(std::string body) { this->body = body; }
  void setSession(Session session) { this->session = session; }
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
  Request getRequest() { return request; }
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

class ServerError : public std::exception
{
private:
  std::string message;
public:
  ServerError():message("ServerError"){}
  ServerError(std::string message):message(message){}
  ServerError(const ServerError &s):ServerError(s.message){}
  
  static void throwFor(std::string cause)
  {
    std::stringstream ret;
    ret << "Exception ServerError thrown for cause: " << cause;
    std::string message = ret.str();
    throw ServerError(message);
  }

  static void throwFor(std::string cause, Request &r)
  {
    std::stringstream ret;
    ret << "Exception ServerError thrown for request:[" << r.getMethod() << "] " << r.getPath() << " cause: " << cause;
    std::string message = ret.str();
    throw ServerError(message);
  }

  virtual const char *what() const _NOEXCEPT
  {
    return message.c_str();
  }
};

} // namespace server
#endif