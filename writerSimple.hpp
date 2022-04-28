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
   // virtual void writeFile(string,NetworkLayout,Interlock,map<int,string>,map<int,string>,int) override;
    void writeFile(string, ParserXML *pXML,int) override;
    friend class writer;
};
