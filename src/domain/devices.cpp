#include "domain.h"
#include "json.h"

int server::DevicesResponse::on(std::stringstream &out)
{
    headers[HEADER_CONTENT_TYPE] = CONTENT_TYPE_JSON;

    Device device1("87b89as793b937cv5", 10);
    Device device2("8sdaf7assa3b937cv", 2);
    std::vector<Device> devices;

    devices.push_back(device1);
    devices.push_back(device2);
    devices.push_back(device2);
    devices.push_back(device1);
    
    out<<json::JSON()
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
    .end()
    .str();
    return 200;
}