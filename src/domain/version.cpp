#include "domain.h"

int server::VersionResponse::on(std::stringstream &out)
{
    headers[HEADER_CONTENT_TYPE] = CONTENT_TYPE_JSON;
    out << "{ \
\"version\" : \""
        << FIRMWARE_VERSION << "\" \
}";
    return 200;
}
