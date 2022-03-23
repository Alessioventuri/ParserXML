#pragma once
#include <string>
#include <vector>
#include <set>
#include "Signal.hpp"
#include "Linear.hpp"
#include "Point.hpp"
#include "Route.hpp"

class NetworkLayout{
private:
	vector<Point> points;
	vector<Linear> linears;
	vector<Signal> signals;
    string pointString(map<int,string>);
    string linearString(map<int,string>);
    string signalString(map<int,string>,map<int,string>);
    string pointStringAdaptive(Route route,map<int,string>);
    string linearStringAdaptive(Route route,map<int,string>);
    string signalStringAdaptive(Route route,map<int,string>,map<int,string>);
public:
    NetworkLayout(){};
    vector<Point> getPoints(){return points;}
	vector<Linear> getLinears(){return linears;}
	vector<Signal> getSignals(){return signals;}
    string toString(map<int,string>,map<int,string>);
    string toStringAdaptive(Route,map<int,string>,map<int,string>);
    void addSignal(int &id, int &section, string &linearEnd);
    void addLinear(int &id, int &up, int &down);
    void addPoint(int &id, int &stem, int &plus, int &minus);

};
