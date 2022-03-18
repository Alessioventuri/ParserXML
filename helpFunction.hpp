#pragma once
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"
#include "Route.hpp"
#include "NetworkLayout.hpp"
#include <string>

using namespace rapidxml;


// int getIntFromNeighborPoint(xml_node<> *neighbor);
// int getIntFromNeighborLinear(xml_node<> *neighbor);
map<int,string> getCorrispondenceLinearPoint(Route &route, NetworkLayout &nl, map<int,string> &plC);
string getCorrispondenceSignal(Route &route, NetworkLayout &nl, map<int,string> &sC);

//TODO: add signal src and dest to the corrispondenceSignal ???!!?!

string getCorrispondenceSignal(Route &route,NetworkLayout &nl,map<int,string> &sC)
{
    map<string,string> corrispondence;
    string output;
    for(int i =0; i < route.getSignals().size();i++){
        if(route.getSignals().at(i) == true)
            corrispondence.insert(pair<string,string>(sC.at(nl.getSignals().at(i).getMbId()),to_string(nl.getSignals().at(i).getMbId()))); 
    }
    for(auto it = corrispondence.begin();it!= corrispondence.end();++it){
        output += "\nName : " + it->first + " -> id : " + it->second; 
    }
    output += "\n";
    return output;
}

map<int,string> getCorrispondenceLinearPoint(Route &route,NetworkLayout &nl,map<int,string> &plC)
{
    map<int,string> corrispondence;
    string output;
    for(int i =0; i < route.getPoints().size();i++){
        if(route.getPoints().at(i) != "INTER"){
            corrispondence.insert(pair<int,string>(nl.getPoints().at(i).getSectionId(),plC.at(nl.getPoints().at(i).getSectionId())));
            corrispondence.insert(pair<int,string>(nl.getPoints().at(i).getStem(),plC.at(nl.getPoints().at(i).getStem())));
            corrispondence.insert(pair<int,string>(nl.getPoints().at(i).getPlus(),plC.at(nl.getPoints().at(i).getPlus())));
            corrispondence.insert(pair<int,string>(nl.getPoints().at(i).getMinus(),plC.at(nl.getPoints().at(i).getMinus())));
        }
    } 
    for(int i =0; i < route.getPath().size();i++){
        if(route.getPath().at(i) != -1){
            corrispondence.insert(pair<int,string>(nl.getLinears().at(i).sectionId,plC.at(nl.getLinears().at(i).sectionId)));
            if(nl.getLinears().at(i).getUpNeig() != -1)
                corrispondence.insert(pair<int,string>(nl.getLinears().at(i).getUpNeig(),plC.at(nl.getLinears().at(i).getUpNeig())));
            if(nl.getLinears().at(i).getDownNeig() != -1)
                corrispondence.insert(pair<int,string>(nl.getLinears().at(i).getDownNeig(),plC.at(nl.getLinears().at(i).getDownNeig())));
        }
    }
    return corrispondence;
}
