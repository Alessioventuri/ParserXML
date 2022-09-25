#pragma once
#include <iostream>
#include <string>
#include "ParserXML.hpp"
#include <map>

enum fileType{
    UMCFile,SimpleFile
};

class writer {
public:
    writer() = default;
    virtual ~writer() = default; // Destructor of base class must always be virtual 
    virtual void writeFile(string,ParserXML *,int train = 1,int select = 0,int route1 = 0, int route = 0) = 0;
    static writer* write(fileType type);
};


