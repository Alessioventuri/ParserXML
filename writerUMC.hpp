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
    string defaultUMCsetup(NetworkLayout,Interlock,int,map<int,string>,map<int,string>);
    string pointObjectUMC(Route);
    string linearObjectUMC(Route,map<int,string>,map<int,string>,NetworkLayout);
    string signalObjectUMC();
    string findMb(Route ,NetworkLayout ,int ,map<int,string>);

};
