#pragma once
#include "writer.hpp"
#include "ParserXML.hpp"
#include <string>
#include <iostream>
#include <fstream>

class writerUMC : public writer{
private :
    // ONE TRAIN
    string defaultUMCsetupOneRoute(ParserXML*,int);
    string pointObjectUmcOneRoute(Route,map<int,string>,NetworkLayout);
    string linearObjectUmcOneRoute(Route,map<int,string>,map<int,string>,NetworkLayout);
    string signalObjectUmcOneRoute(Route, map<int,string>,map<int,string>,NetworkLayout);
    string trainObjectUmcOneRoute(Route,map<int,string>,NetworkLayout);

    string findMb(Route ,NetworkLayout ,int ,map<int,string>);

    string abstractionUmcOneRoute(Route,map<int,string>,map<int,string>,NetworkLayout);
    string derailAbsOneRoute(Route,map<int,string>,NetworkLayout);
    string brokenSignalsOneRoute(Route,map<int,string>,map<int,string>,NetworkLayout);

    string defaultUMCsetupTwoRoute(ParserXML*,int);

    // TWO TRAINs

    string defaultUMCsetupTwoRoute(ParserXML*,int,int);
    string stringCombinerNl(int, int,ParserXML *);
    string stringCombinerIl(int, int,ParserXML *);
    string stringCombinerId(int,int,ParserXML *);

    // string pointObjectUmcTwoRoute(Route,map<int,string>,NetworkLayout);
    // string linearObjectUmcTwoRoute(Route,map<int,string>,map<int,string>,NetworkLayout);
    // string signalObjectUmcTwoRoute(Route, map<int,string>,map<int,string>,NetworkLayout);
    // string trainObjectUmcTwoRoute(Route,map<int,string>,NetworkLayout);

    // string findMb(Route ,NetworkLayout ,int ,map<int,string>);

    // string abstractionUmcTwoRoute(Route,map<int,string>,map<int,string>,NetworkLayout);
    // string derailAbsTwoRoute(Route,map<int,string>,NetworkLayout);
    // string brokenSignalsTwoRoute(Route,map<int,string>,map<int,string>,NetworkLayout);

public:
    writerUMC(){};
    ~writerUMC() = default;
    //virtual void writeFile(string,NetworkLayout,Interlock,map<int,string>,map<int,string>,int) override;
    void writeFile(string,ParserXML*,int);

    friend class writer;

};
