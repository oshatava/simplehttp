#include "domain.h"
#include "html.h"

server::Response domain::My404ErrorPage(server::Request &request)
{      
      return server::Response(request)
      .setHeader(HEADER_CONTENT_TYPE, CONTENT_TYPE_TEXT)
      .setBody(html::HTML()
        .tag("head")
          .tag("meta").param("http-equiv", "Content-Type").param("charset","utf-8").end()
          .tag("title").body("My 404 Error Page").end()
        .end()
        .tag("body")
          .tag("h1").body("404 Error").end()
          .tag("p")            
            .body("Страница - '")
            .tag("b").body(request.getPath()).end()
            .body("' не найдена!")
          .end()
        .end()  
      .build())
      .setRetCode(RESPONSE_CODE_ERROR_404);
}