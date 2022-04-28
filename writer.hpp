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
    // Destructor of base class must always be virtual 
    virtual ~writer() = default; 
    virtual void writeFile(string,ParserXML *,int) = 0;
    static writer* write(fileType type);
};


