#pragma once
#include "Interlock.hpp"
#include "rapidxml.hpp"
#include <iostream>
#include <unordered_map>

using namespace rapidxml;
using namespace std;

class ParserXML{
public:
    // cppcheck-suppress  noExplicitConstructor 
    ParserXML(string input);    
    int getIntFromNeighborLinear(xml_node<> *neighbor);
    int getIntFromNeighborPoint(xml_node<> *neighbor);
    string SplitFilename(string str);
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
    int getSecondRoute(int i);
    void setIl(const Interlock &i){this->il = i;}

private:
    static int count;
    int secondRoute;
    string input,output;
    Interlock il;
    NetworkLayout nl;
    map<int, string> plCorrispondence;
    map<int, string> mbCorrispondence;
    unordered_map<string, int> id;
};