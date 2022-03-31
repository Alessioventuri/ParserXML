#pragma once
#include <vector>
#include "Route.hpp"
#include "NetworkLayout.hpp"

using namespace std;

class Interlock{
private:
    vector<Route> routes;
	int maxPathlength;
	int maxChunk;
    	
public:
    Interlock(){};
    void generateMaxChunk();
    void addRoute(Route &rou);
    void deleteRoute(Route &rou,int i);
    void setMaxValues(int &maxpath);
    string toString();
    string toStringCombiner();
    int getMaxPathLength(){ return maxPathlength;}
    int getMaxChunk(){return maxChunk;}
    void getRoutesDisplay();
    vector<Route> getRoutes(){ return routes;}
    Interlock routeCombiner(NetworkLayout nl,Interlock il);

};
