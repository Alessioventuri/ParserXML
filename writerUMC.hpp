#pragma once
#include "writer.hpp"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class writerUMC : public writer
{
public:
    writerUMC(){};
    void writeFile(string,NetworkLayout,Interlock,map<int,string>,map<int,string>);
    friend class writer;
    void defaultUMCsetup();
};
