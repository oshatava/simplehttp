#include "domain.h"
#include "json.h"

server::Response domain::VersionResponse(server::Request &request)
{
    return server::Response(request)
        .setHeader(HEADER_CONTENT_TYPE, CONTENT_TYPE_JSON)
        .setBody(json::JSON().value("version", FIRMWARE_VERSION).end().str())
        .setRetCode(RESPONSE_CODE_OK_200);
}
