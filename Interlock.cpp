#include "Interlock.hpp"
#include "helpFunction.hpp"
#include <iostream>
using namespace std;

	
void Interlock::addRoute(Route &rou){
	routes.push_back(rou);
}

void Interlock::deleteRoute(Route &rou,int i){
	routes.erase(routes.begin()+i);
}
	
void Interlock::setMaxValues(int &maxpath) {
		maxPathlength = maxpath;

}
void Interlock::generateMaxChunk(){
		for (int i = 0; i < routes.size(); i++) {
			if (maxChunk < routes.at(i).getCheckPoints().size()){
				maxChunk = routes.at(i).getCheckPoints().size();
			}
		}

}
string Interlock::toString() {
	generateMaxChunk();
	if (routes.size() == 0 && maxChunk == 0 && maxPathlength == 0) 
		return "";
	string output = "value\n";
	
	output += "maxRoutes: Int = " + to_string(routes.size()) + ",\n" +
			"maxPathLength: Int = " + to_string(maxPathlength) + ",\n" +
			"maxChunks: Int = " + to_string((maxChunk-1)) + "\n";
	
	
	output += "axiom\n";
	for ( int i = 0 ; i < routes.size(); i++ )
		output += routes.at(i).toString(maxPathlength, maxChunk) + ",\n";
	if (output.length() > 0)
		output = output.substr(0, output.length() - 2);
	return output;
}
string Interlock::toStringCombiner() {
	generateMaxChunk();
	if (routes.size() == 0 && maxChunk == 0 && maxPathlength == 0) 
		return "";
	string output = "value\n";
	
	output += "maxRoutes: Int = " + to_string(routes.size()) + ",\n" +
			"maxPathLength: Int = " + to_string(maxPathlength) + ",\n" +
			"maxChunks: Int = " + to_string((maxChunk-1)) + "\n";
	output += "axiom\n";
	for ( int i = 0 ; i < routes.size(); i++ )
		output += routes.at(i).toString(maxPathlength) + ",\n";
	if (output.length() > 0)
		output = output.substr(0, output.length() - 2);
	return output;
}

void Interlock::getRoutesDisplay()
{
	for(auto it = routes.begin(); it!=routes.end();++it){
		cout << (*it).getDest() << " destination"
		<< (*it).getSrc() << " : source" << endl;
	}
}
Interlock Interlock::routeCombiner(NetworkLayout nl, Interlock il){
    Interlock new_Il;
    int size = il.getRoutes().size();
    for(int i = 0; i < size;i++){
        if(il.getRoutes().at(i).getPath().size() < 4){
            for(int j = 0; j < size;j++){   
                if(il.getRoutes().at(i).getPath().back() == il.getRoutes().at(j).getPath().at(0) and
                il.getRoutes().at(i).getDirection() == il.getRoutes().at(j).getDirection()){
                    Route route1(il.getRoutes().at(i));
                    Route route2(il.getRoutes().at(j));
                    int id = route2.getRouteId()*100+route1.getRouteId();
                    Route routeCombined(id,
                            route1.getSrc(),
                            route2.getDest(),
                            route1.getDirection(),
                            pointsCombiner(route1.getPoints(),route2.getPoints()),
                            pathCombiner(route1.getPath(),route2.getPath()),
                            signalsCombiner(route1.getSignals(),route2.getSignals()),
                            overlapCombiner(route1.getOverlap(),route2.getOverlap()),
                            conflictCombiner(route1.getConflict(),route2.getConflict()),
                            route1.getPoints().size());
                    new_Il.addRoute(routeCombined);
                }
            }   
        }
    }
    int length = il.getMaxPathLength()+3;
    new_Il.setMaxValues(length);
    return new_Il;
}

	