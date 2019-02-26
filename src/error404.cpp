#include "domain.h"
#include "html.h"

int server::My404ErrorPage::on(std::stringstream &out)
{
      headers[HEADER_CONTENT_TYPE] = CONTENT_TYPE_TEXT;      
      out<<html::HTML()
        .tag("head")
          .tag("meta").param("http-equiv", "Content-Type").end()
          .tag("title").body("My 404 Error Page").end()
        .end()
        .tag("body")
          .tag("h1").body("404 Error").end()
          .tag("p")
            .body("Page - '")
            .tag("b").body(request.getPath()).end()
            .body("' not found")
          .end()
        .end()  
      .build();
      return RESPONSE_CODE_ERROR_404;
}