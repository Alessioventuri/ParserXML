#include <string>
#include <iostream>
#include "NetworkLayout.hpp"


using namespace std;

string NetworkLayout::pointString(map<int,string> plC){
    string output;
    for ( int i = 0 ; i < points.size(); i++ )
        output += points.at(i).toString(plC) + ",\n";
    if (output.length() > 0)
        output = output.substr(0, output.length() - 2);
    return output;
}
string NetworkLayout::pointStringAdaptive(Route route,map<int,string> pl){
    string output; 
    for(int i = 0; i < route.getPoints().size();i++){
        if(route.getPoints().at(i) != "INTER")
            output += points.at(i).toString(pl) +",\n";
    }
    if (output.length() > 0)
        output = output.substr(0, output.length() - 2);
    return output;
}

string NetworkLayout::linearString(map<int,string> plC){
    string output;
    for ( int i = 0 ; i < linears.size(); i++ )
        output += linears.at(i).toString(plC) + ",\n";
    if (output.length() > 0)
        output = output.substr(0, output.length() - 2);
    return output;
}
string NetworkLayout::linearStringAdaptive(Route route,map<int,string> plC){
    string output;
    for(int i = 0; i < route.getPath().size();i++){
        //path contains index of linears and linears start when points end 
        //(ie. path{8},points has index (0,1,2,3,4), and linears start from 5, so path.at(i)-points.size() ( 8-5 = 3) 
        if(route.getPath().at(i) >= points.size())
            output += linears.at(route.getPath().at(i)-points.size()).toString(plC) +",\n";
    }
    if (output.length() > 0)
        output = output.substr(0, output.length() - 2);
    return output;
}

string NetworkLayout::signalString(map<int,string> plC,map<int,string> sC){
    string output;
    for ( int i = 0 ; i < signals.size(); i++ )
        output += signals.at(i).toString(plC,sC) + ",\n";
    if (output.length() > 0)
        output = output.substr(0, output.length() - 2);
    return output;
}
string NetworkLayout::signalStringAdaptive(Route route,map<int,string> sC,map<int,string> plC){
    string output;
    for(int i = 0; i< route.getSignals().size();i++){
        if(route.getSignals().at(i) == true)
            output += signals.at(i).toString(plC,sC) + ",\n";
    }
    if (output.length() > 0)
        output = output.substr(0, output.length() - 2);
    return output;
}

string NetworkLayout::toString(map<int,string> plC,map<int,string> sC){
    string s;
    s += "value\nmaxLinear: Int = " + to_string(linears.size()) + ",\n";
    s += "maxPoint: Int = " + to_string(points.size()) + ",\n";
    s += "maxMb: Int = " + to_string(signals.size()) +"\n";
    s += "axiom\n";
    s += pointString(plC);
    
    if (points.size() != 0)
        s += ",\n\n";
    s += linearString(plC);
    s += ",\n\n";
    s += signalString(plC,sC); 
    return s;
}
string NetworkLayout::toStringAdaptive(Route route,map<int,string> plC,map<int,string> sC){
    string s;
    s += "POINT[NAME-ID] = LINEAR NAME-ID\n";
    s += pointStringAdaptive(route,plC);
    s += ",\n\n";
    s += "LINEAR[NAME-ID] = UP/DOWN NAME-ID\n";
    s += linearStringAdaptive(route,plC);
    s += ",\n\n";
    s += "SIGNAL[NAME-ID] holds on LINEAR NAME-ID\n";
    s += signalStringAdaptive(route,sC,plC);
    return s;
}

void NetworkLayout::addSignal(int &id, int &section, string &linEnd){
    Signal s(id, section, linEnd);
    signals.push_back(s);
}
void NetworkLayout::addLinear(int &id, int &up, int &down){
    Linear linear(id, up, down);
    linears.push_back(linear);
}
void NetworkLayout::addPoint(int &id, int &stem, int &plus, int &minus){
    Point point(id, stem, plus, minus);
    points.push_back(point);
}




