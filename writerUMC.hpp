#pragma once
#include "writer.hpp"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class writerUMC : public writer
{
private :
    string defaultUMCsetup(NetworkLayout,Interlock,int,map<int,string>,map<int,string>);
    string pointObjectUMC(Route,map<int,string>,NetworkLayout);
    string linearObjectUMC(Route,map<int,string>,map<int,string>,NetworkLayout);
    string signalObjectUMC(Route, map<int,string>,map<int,string>,NetworkLayout);
    string findMb(Route ,NetworkLayout ,int ,map<int,string>);
    string trainObjectUMC(Route,map<int,string>,NetworkLayout);
    string abstractionUMC(Route,map<int,string>,map<int,string>,NetworkLayout);
public:
    writerUMC(){};
    ~writerUMC() = default;
    void writeFile(string,NetworkLayout,Interlock,map<int,string>,map<int,string>);
    friend class writer;

};
