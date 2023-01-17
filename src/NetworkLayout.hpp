#pragma once
#include <vector>
#include <set>
#include "Signal.hpp"
#include "Linear.hpp"
#include "Point.hpp"
#include "Route.hpp"

class NetworkLayout{
private:
	std::vector<Point> points;
	std::vector<Linear> linears;
	std::vector<Signal> signals;
    std::string pointString(std::map<int,std::string>);
    std::string linearString(std::map<int,std::string>);
    std::string signalString(std::map<int,std::string>,std::map<int,std::string>);
    std::string pointStringAdaptive(Route route,std::map<int,std::string>);
    std::string linearStringAdaptive(Route route,std::map<int,std::string>);
    std::string signalStringAdaptive(Route route,std::map<int,std::string>,std::map<int,std::string>);
public:
    NetworkLayout() = default;
    std::vector<Point> getPoints(){return points;}
	std::vector<Linear> getLinears(){return linears;}
	std::vector<Signal> getSignals(){return signals;}
    std::string toString(std::map<int,std::string>,std::map<int,std::string>);
    std::string toStringAdaptive(Route,std::map<int,std::string>,std::map<int,std::string>);
    void addSignal(const int &id,const int &section,const std::string &linearEnd);
    void addLinear(int &id,const int &up,const int &down);
    void addPoint(int &id,const int &stem,const int &plus,const int &minus);

};
