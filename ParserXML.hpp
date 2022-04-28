#pragma once
#include "Interlock.hpp"
#include "NetworkLayout.hpp"
#include <stdexcept>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"
#include <unordered_map>
#include <string>
#include <cstring>
#include <exception>

#include <sys/stat.h>
#include <sys/types.h>
#include <sstream>


using namespace rapidxml;
using namespace std;

class ParserXML{
public:
    int getIntFromNeighborLinear(xml_node<> *neighbor);
    int getIntFromNeighborPoint(xml_node<> *neighbor);
    string SplitFilename(string str);
    ParserXML(string input);
    ~ParserXML();
    void searchPoints(xml_node<> *network_node);
    void searchLinears(xml_node<> *network_node);
    void searchSignals(xml_node<> *network_node);
    void NetworkLayoutProcess(xml_node<> *network_node);
    void InterlockingProcess(xml_node<> *network_node);
    string getOutput(){ return output;}
    Interlock getIl(){return this->il;};
    NetworkLayout getNl(){return this->nl;}
    map<int, string> getPlCorrispondence(){return this->plCorrispondence;}
    map<int, string> getMbCorrispondence(){return this->mbCorrispondence;}
    unordered_map<string,int> getIdMap(){return this->id;}
    void setIl(Interlock &i){this->il = i;}

private:
    static int count;
    string input,output;
    Interlock il;
    NetworkLayout nl;
    map<int, string> plCorrispondence;
    map<int, string> mbCorrispondence;
    unordered_map<string, int> id;
};