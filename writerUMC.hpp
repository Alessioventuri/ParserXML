#include "writer.hpp"
#include <string>
#include <iostream>
#include <fstream>

class writerUMC : public writer{
private :
    // ONE TRAIN
    string defaultUMCsetupOneRoute(ParserXML*,int);

    string pointObjectUmcOneRoute (Route, map<int,string>, NetworkLayout);
    string linearObjectUmcOneRoute(Route, int, map<int,string>, map<int,string>, NetworkLayout);
    string signalObjectUmcOneRoute(Route, map<int,string>, map<int,string>, NetworkLayout);
    string trainObjectUmcOneRoute (Route, int, map<int,string>);

    string findMb                 (Route, NetworkLayout, int, map<int,string>);

    string abstractionUmcOneRoute (Route, int, map<int,string>, map<int,string>, NetworkLayout);
    string derailAbsOneRoute      (Route, int, map<int,string>, NetworkLayout);
    string brokenSignalsOneRoute  (Route, int, map<int,string>, map<int,string>, NetworkLayout);
    string trainArrived           (Route, int, map<int,string>);

    // TWO TRAINs
    string defaultUMCsetupTwoRoute(ParserXML *, int);
    string defaultUMCsetupTwoRoute(ParserXML *, int, int);
    string stringCombinerNl       (ParserXML *, int, int);
    string stringCombinerIl       (ParserXML *, int, int);
    string stringCombinerId       (ParserXML *, int, int);
    string abstractionUmcTwoRoute (Route, Route , int, int, map<int,string>, map<int,string>, NetworkLayout );
    string pointObjectUmcTwoRoute (Route &, Route &, map<int,string> , NetworkLayout );
    string linearObjectUmcTwoRoute(Route, Route , int, map<int,string>, map<int,string>, NetworkLayout);
    string trainObjectUmcTwoRoute (Route, Route , int, map<int,string>);


public:
    writerUMC(){};
    ~writerUMC() = default;
    void writeFile                (string,ParserXML*,int,int,int,int) override;

    friend class writer;

};
