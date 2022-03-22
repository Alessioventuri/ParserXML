#pragma once
#include <iostream>
#include <string>
#include "NetworkLayout.hpp"
#include "Interlock.hpp"
#include <map>

enum fileType{
    UMCFile,po
};

class writer {
public:
    writer() = default;
    // Destructor of base class must always be virtual 
    virtual ~writer() = default;  
    virtual void writeFile(string,NetworkLayout,Interlock,map<int,string>,map<int,string>) = 0;
    static writer* write(fileType type);
};

