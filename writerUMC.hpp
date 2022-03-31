#pragma once
#include "writer.hpp"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class writerUMC : public writer
{
private :
    string defaultUMCsetupOneRoute(NetworkLayout,Interlock,int,map<int,string>,map<int,string>);
    string pointObjectUmcOneRoute(Route,map<int,string>,NetworkLayout);
    string linearObjectUmcOneRoute(Route,map<int,string>,map<int,string>,NetworkLayout);
    string signalObjectUmcOneRoute(Route, map<int,string>,map<int,string>,NetworkLayout);
    string trainObjectUmcOneRoute(Route,map<int,string>,NetworkLayout);

    string findMb(Route ,NetworkLayout ,int ,map<int,string>);

    string abstractionUmcOneRoute(Route,map<int,string>,map<int,string>,NetworkLayout);
    string derailAbsOneRoute(Route,map<int,string>,NetworkLayout);
    string brokenSignalsOneRoute(Route,map<int,string>,map<int,string>,NetworkLayout);

public:
    writerUMC(){};
    ~writerUMC() = default;
    void writeFile(string,NetworkLayout,Interlock,map<int,string>,map<int,string>,int);
    friend class writer;

};
