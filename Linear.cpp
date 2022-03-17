#include "Linear.hpp"
using namespace std;

string Linear::toString(){
	string secId = to_string(sectionId);
	string upN = to_string(upNeighbour);
	string downN = to_string(downNeighbour);
	return  "linearUp[" + secId + "] = " + upN + ",\nlinearDown[" + secId + "] = " + downN;
}

string Linear::toString(map<int,string> pl){
	string secId = pl.find(sectionId)->second;
	string upN =  upNeighbour != -1 ? pl.find(upNeighbour)->second : "null";
	string downN = downNeighbour != -1 ? pl.find(downNeighbour)->second : "null";
	return  "linearUp[" + secId + " - "+ to_string(sectionId)+ "] = " + upN + " - " + to_string(upNeighbour)
			 + ",\nlinearDown[" + secId + " - "+ to_string(sectionId)+ "] = " + downN + " - " + to_string(downNeighbour);

}
