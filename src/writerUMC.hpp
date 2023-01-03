#include "writer.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <memory>


class writerUMC : public writer{
private :
    // ONE TRAIN
    string defaultUMCsetupOneRoute(unique_ptr<ParserXML>&,int);

    string pointObjectUmcOneRoute (Route, map<int,string>, NetworkLayout);
    string linearObjectUmcOneRoute(Route, int, map<int,string>, map<int,string>, NetworkLayout);
    string signalObjectUmcOneRoute(Route, map<int,string>, map<int,string>, NetworkLayout);
    string trainObjectUmcOneRoute (Route, int, map<int,string>);

    string findMb                 (Route, NetworkLayout, int, map<int,string>);

    string abstractionUmcOneRoute (Route, int, map<int,string>, map<int,string>, NetworkLayout);
    string derailAbsOneRoute      (Route, int, map<int,string>, NetworkLayout);
    string brokenSignalsOneRoute  (Route, int, map<int,string>, map<int,string>, NetworkLayout);
    string trainArrived           (Route, int, map<int,string>) const;

    // TWO TRAINs
    string defaultUMCsetupTwoRoute(unique_ptr<ParserXML>&, int);
    string defaultUMCsetupTwoRoute(unique_ptr<ParserXML>&, int, int);
    string stringCombinerNl       (unique_ptr<ParserXML>&, int, int) const;
    string stringCombinerIl       (unique_ptr<ParserXML>&, int, int) const;
    string stringCombinerId       (unique_ptr<ParserXML>&, int, int) const;
    string abstractionUmcTwoRoute (const Route&,const Route&, int, int, map<int,string>, map<int,string>, NetworkLayout );
    string pointObjectUmcTwoRoute (Route &, Route &, map<int,string> , NetworkLayout ) const;
    string linearObjectUmcTwoRoute(Route, Route , int, map<int,string>, map<int,string>, NetworkLayout);
    string trainObjectUmcTwoRoute (Route, Route , int, map<int,string>);


public:
    writerUMC() = default;
    ~writerUMC() = default;
    void writeFile                (string,unique_ptr<ParserXML>&,int,int,int,int) override;

    friend class writer;

};
