#pragma once
#include "Interlock.hpp"
#include "../rapidxml/rapidxml.hpp"
#include <iostream>
#include <unordered_map>
#include <nlohmann/json.hpp>

using json = nlohmann::json;


using namespace rapidxml;
using namespace std;

class ParserXML{
public:
    // cppcheck-suppress  noExplicitConstructor 
    ParserXML() = default;
    ParserXML(string input);    
    int getIntFromNeighborLinear(xml_node<> *neighbor) const;
    int getIntFromNeighborPoint(xml_node<> *neighbor) const;
    int countRoutes(string input);
    string SplitFilename(string str) const;
    void searchPoints(xml_node<> *network_node);
    void searchLinears(xml_node<> *network_node);
    void searchSignals(xml_node<> *network_node);
    void NetworkLayoutProcess(xml_node<> *network_node);
    void InterlockingProcess(xml_node<> *network_node);
    string getOutput() const { return output;}
    Interlock getIl() const {return this->il;};
    NetworkLayout getNl() const {return this->nl;}
    map<int, string> getPlCorrispondence() const {return this->plCorrispondence;}
    map<int, string> getMbCorrispondence() const {return this->mbCorrispondence;}
    unordered_map<string,int> getIdMap() const {return this->id;}
    int getSecondRoute(int i);
    void setIl(const Interlock &i){this->il = i;}
    void setCount(int value) { count = value; }
    std::vector<std::vector<std::string>> get_vector_from_il();
    std::vector<std::vector<std::string>> get_vector_from_network();
    std::vector<std::vector<std::string>> get_signals_from_network();

private:
    static int count;
    int secondRoute;
    bool get_json = false;
    string input,output;
    Interlock il;
    NetworkLayout nl;
    map<int, string> plCorrispondence;
    map<int, string> mbCorrispondence;
    unordered_map<string, int> id;
};