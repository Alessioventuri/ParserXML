#pragma once
#include <map>
#include <vector>
#include <string>
#include <set>
#include <iterator>
#include <iostream>



class Route {
private:
	int routeId;
	int src;
	int destination;
    int lastElem;
    std::string direction;
	std::map<int, std::string> points;
	std::vector<int> path;
    std::map<int, bool> signals;
	std::vector<bool> overlap;
    std::vector<int> checkPoints;
	std::map<int, bool> conflict;
    std::string pathString(int max);
    std::string checkPointString(int max);
    std::string overlapString() const;
    std::string pointString();
    std::string signalString();
    std::string conflictString();
    void generateCheckPoints(int maxPoints,std::vector<int> &path);

public:
    Route(const int &id, int src,[[maybe_unused]] int dest,const std::string &direction, std::map<int,std::string> const &points, std::vector<int>const & path,
        std::map<int,bool> const &signals, std::vector<bool>const & overlap, std::map<int,bool> const &conflict, int maxPoints);
    std::string toString(int maxpath, int maxChunk);
    std::string toString(int maxpath);
    std::vector<int> getCheckPoints() const{return checkPoints;}
    int getRouteId() const{return routeId;}
    int getSrc() const{return src;}
    int getDest() const{return destination;}
    std::vector<bool> &getOverlap(){return overlap;}
    std::string getDirection() const{return direction;}
    std::map<int,bool> getConflict() const{return conflict;}
    std::map<int,std::string> getPoints() const{return points;}
    std::vector<int> getPath() {return path;}
    std::map<int,bool> getSignals() const{return signals;}
    std::string createString(std::vector<int> a,int max) const;
};
