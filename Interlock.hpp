#include <vector>
#include "NetworkLayout.hpp"

using namespace std;

class Interlock{
private:
    vector<Route> routes;
	int maxPathLength;
	int maxChunk;
    	
public:
    Interlock(){
        maxPathLength=0,maxChunk=0;
    };
    void generateMaxChunk();
    void addRoute(Route &rou);
    void deleteRoute(Route &rou,int i);
    void setMaxValues(const int &maxpath);
    void getRoutesDisplay();

    string toString();
    string toStringCombiner();

    int getMaxPathLength(){ return maxPathLength;}
    int getMaxChunk(){return maxChunk;}

    vector<Route> getRoutes(){ return routes;}
    
    Interlock routeCombiner(const NetworkLayout &nl,Interlock il);

};
