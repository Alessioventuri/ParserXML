#pragma once
#include "writer.hpp"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class writerSimple : public writer
{
public:
    writerSimple(){};
    ~writerSimple() = default;
    void writeFile(string,NetworkLayout,Interlock,map<int,string>,map<int,string>);
    friend class writer;
};
