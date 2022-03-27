#include "Interlock.hpp"
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

void Interlock::getRoutesDisplay()
{
	for(auto it = routes.begin(); it!=routes.end();++it){
		cout << (*it).getDest() << " destination"
		<< (*it).getSrc() << " : source" << endl;
	}
}

	