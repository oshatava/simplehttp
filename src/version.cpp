#include "domain.h"
#include "json.h"

int server::VersionResponse::on(std::stringstream &out)
{
    headers[HEADER_CONTENT_TYPE] = CONTENT_TYPE_JSON;
    out << json::JSON().value("version", FIRMWARE_VERSION).end().str();
    return 200;
}
