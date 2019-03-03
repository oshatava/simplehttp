#include "domain.h"
#include "json.h"

server::Response domain::DevicesResponse(server::Request &request)
{
    domain::Device device1("87b89as793b937cv5", 10);
    domain::Device device2("8sdaf7assa3b937cv", 2);
    std::vector<Device> devices;

    devices.push_back(device1);
    devices.push_back(device2);
    devices.push_back(device2);
    devices.push_back(device1);
    
    json::JSON jsn;
    jsn
        .value("version", FIRMWARE_VERSION)
        .value("data", 10)
        .object("object")
            .value("id", 100)
            .value("vv", "100")
            .value("floatVal", 100.4)
            .end()
        .value("floatVal1", 103.0)
        .array("arr")
            .object(device1)
            .object(device2, packDevice)
        .end()
        .array(std::string("arr"))
            .object(device1)
            .object(device2, packDevice)
        .end()        
        .array("arr1", devices, packDevice)
        .array("arr2")
            .objects(devices, packDevice)
        .end()
    .end();

    return server::Response(request)
    .setHeader(HEADER_CONTENT_TYPE, CONTENT_TYPE_JSON)
    .setBody(jsn.str())
    .setRetCode(RESPONSE_CODE_OK_200);
}

server::Response domain::DeviceResponse(server::Request &request){
    json::JSON jsn;
    jsn.value("id", request.getPath());
    for(const auto p:request.getParamsPath()){
        jsn.value(p.first, p.second);
    }

    return server::Response(request)
    .setHeader(HEADER_CONTENT_TYPE, CONTENT_TYPE_JSON)
    .setBody(jsn.value("test", u8"Привет венегрет").end().str())
    .setRetCode(RESPONSE_CODE_OK_200);
}