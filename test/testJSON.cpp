#include <gtest/gtest.h>
#include "json.h"

TEST(JSON, TestEmpty){
    ASSERT_EQ("{}", json::JSON().end().str());    
}

TEST(JSON, TestValueInt){
    int v = 1;
    ASSERT_EQ("{\"v\":1}", json::JSON().value("v", v).end().str());    
}

TEST(JSON, TestValueDouble){
    double v = 1;
    ASSERT_EQ("{\"v\":1}", json::JSON().value("v", v).end().str());    
}

TEST(JSON, TestValueBoolTrue){
    bool v = true;
    ASSERT_EQ("{\"v\":true}", json::JSON().value("v", v).end().str());    
}

TEST(JSON, TestValueBoolFalse){
    bool v = false;
    ASSERT_EQ("{\"v\":false}", json::JSON().value("v", v).end().str());    
}

TEST(JSON, TestValueString){
    const char * v = "v";
    ASSERT_EQ("{\"v\":\"v\"}", json::JSON().value("v", v).end().str());    
}

TEST(JSON, TestObject){    
    ASSERT_EQ("{\"v\":{}}", json::JSON().object("v").end().end().str());    
}

TEST(JSON, TestArray){    
    ASSERT_EQ("{\"v\":[]}", json::JSON().array("v").end().end().str());    
}

TEST(JSON, TestProperties){    
    ASSERT_EQ("{\"v\":\"v\",\"v1\":\"v1\"}", json::JSON().value("v", "v").value("v1", "v1").end().str());    
}