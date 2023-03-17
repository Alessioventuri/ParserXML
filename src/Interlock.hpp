#pragma once
#include <vector>
#include "NetworkLayout.hpp"

class Interlock{
private:
    std::vector<Route> routes;
	int maxPathLength;
	int maxChunk;
    	
public:
    Interlock() :maxPathLength(0),maxChunk(0) {};
    void generateMaxChunk();
    void addRoute(const Route &rou);
    void deleteRoute(int i);
    void setMaxValues(const int &maxpath);
    void getCheckPointsSize(int i);


    std::string toString();
    std::string toStringCombiner();

    int getMaxPathLength() const { return maxPathLength;}
    int getMaxChunk() const {return maxChunk;}

    std::vector<Route> getRoutes() const{ return routes;}
    std::map<int,bool> conflictCombiner(std::map<int,bool> m1, std::map<int,bool> m2);
    std::map<int,bool> signalsCombiner(std::map<int,bool> s1, std::map<int,bool> s2);
    std::map<int,std::string> pointsCombiner(std::map<int,std::string> s1, std::map<int,std::string> s2);
    std::vector<int> pathCombiner(std::vector<int> v1, std::vector<int> v2);
    std::vector<bool> overlapCombiner(std::vector<bool> v1, std::vector<bool> &v2);

    
    Interlock routeCombiner(Interlock il);
};
