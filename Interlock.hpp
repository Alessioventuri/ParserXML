#include <vector>
#include "NetworkLayout.hpp"

class Interlock{
private:
    vector<Route> routes;
	int maxPathLength;
	int maxChunk;
    	
public:
    Interlock(){
        maxPathLength=0;
        maxChunk=0;
    };
    void generateMaxChunk();
    void addRoute(const Route &rou);
    void deleteRoute(int i);
    void setMaxValues(const int &maxpath);
    void getRoutesDisplay();
    void getCheckPointsSize(int i);


    string toString();
    string toStringCombiner();

    int getMaxPathLength() const{ return maxPathLength;}
    int getMaxChunk() const {return maxChunk;}

    vector<Route> getRoutes() const { return routes;}
    
    Interlock routeCombiner(Interlock il) const ;
};
