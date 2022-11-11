#pragma once
#include <map>
#include <vector>
#include <string>
#include <set>
#include <iterator>
#include <iostream>


using namespace std;

class Route {
private:
	int routeId;
	int src;
	int destination;
    int lastElem;
    string direction;
	map<int, string> points;
	map<int, bool> signals;
	vector<int> path;
	vector<bool> overlap;
    vector<int> checkPoints;
	map<int, bool> conflict;
    string pathString(int max);
    string checkPointString(int max);
    string overlapString() const;
    string pointString();
    string signalString();
    string conflictString();
    void generateCheckPoints(int maxPoints,vector<int> &path);

public:
    Route(const int &id, int src,[[maybe_unused]] int dest,const string &direction, map<int,string> const &points, vector<int>const & path,
        map<int,bool> const &signals, vector<bool>const & overlap, map<int,bool> const &conflict, int maxPoints);
    string toString(int maxpath, int maxChunk);
    string toString(int maxpath);
    vector<int> getCheckPoints() const{return checkPoints;}
    int getRouteId() const{return routeId;}
    int getSrc() const{return src;}
    int getDest() const{return destination;}
    vector<bool> &getOverlap(){return overlap;}
    string getDirection() const{return direction;}
    map<int,bool> getConflict() const{return conflict;}
    map<int,string> getPoints() const{return points;}
    vector<int> getPath() const{return path;}
    map<int,bool> getSignals() const{return signals;}
    string createString(vector<int> a,int max) const;
};
