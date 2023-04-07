#pragma once
#include <iostream>
#include <string>
#include "ParserXML.hpp"
#include <map>
#include <memory>

enum fileType{
    UMCFile,SimpleFile
};

class writer {
public:
    writer() = default;
    virtual ~writer() = default; // Destructor of base class must always be virtual 
    virtual void writeFile(string,unique_ptr<ParserXML>&,int train ,int select = 0,int route1 = 0, int route = 0) = 0;
    static unique_ptr<writer> write(fileType type);

};


