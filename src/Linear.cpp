#include "Linear.hpp"
using namespace std;

string Linear::toString(const map<int,string> &plC) const{
	string secId = plC.find(sectionId)->second;
	string upN =  upNeighbour != -1 ? plC.find(upNeighbour)->second : "null";
	string downN = downNeighbour != -1 ? plC.find(downNeighbour)->second : "null";
	return  "linearUp[" + secId + " - "+ to_string(sectionId)+ "] = " + upN + " - " + to_string(upNeighbour)
			 + ",\nlinearDown[" + secId + " - "+ to_string(sectionId)+ "] = " + downN + " - " + to_string(downNeighbour);

}
