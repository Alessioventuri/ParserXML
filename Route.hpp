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
    string overlapString();
    string pointString();
    string signalString();
    string conflictString();
    void generateCheckPoints(int maxPoints,vector<int> &path);

public:
    Route(const int &id, int src, int dest,const string &direction, map<int,string> const &points, vector<int>const & path,
        map<int,bool> const &signals, vector<bool>const & overlap, map<int,bool> const &conflict, int maxPoints);
    string toString(int maxpath, int maxChunk);
    string toString(int maxpath);
    vector<int> getCheckPoints(){return checkPoints;}
    int getRouteId(){return routeId;}
    int getSrc() {return src;}
    int getDest() {return destination;}
    vector<bool> &getOverlap(){return overlap;}
    string getDirection(){return direction;}
    map<int,bool> getConflict(){return conflict;}
    map<int,string> getPoints(){return points;}
    vector<int> getPath(){return path;}
    map<int,bool> getSignals(){return signals;}
    string createString(vector<int> a,int max);
};
