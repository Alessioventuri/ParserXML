#pragma once
#include <vector>
#include "Route.hpp"

using namespace std;

class Interlock{
private:
    vector<Route> routes;
	int maxPathlenght;
	int maxChunk;
    	
public:
    Interlock(){};
    void generateMaxChunk();
    void addRoute(Route &rou);
    void setMaxValues(int &maxpath);
    string toString();
    int getMaxPathLenght(){ return maxPathlenght;}
    int getMaxChunk(){return maxChunk;}
    void getRoutesDisplay();
    vector<Route> getRoutes(){ return routes;}
};
