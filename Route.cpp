#include "Route.hpp"


Route::Route(int &id_,int src_, int dest_,string dire_, map<int,string> points_, vector<int> path_,
 	map<int,bool> signals_, vector<bool> overlap_, map<int,bool> conflict_, int maxPoints_){
     	routeId = id_;
		src = src_;
		direction = dire_;
		destination = dest_;
		points = points_;
		signals = signals_;
		conflict = conflict_;
		path = path_;
		overlap = overlap_;
		lastElem = path.at(path.size() -1);

		for (int i = 0; i < path.size(); i++) {
			if (path.at(i) == -1){
				lastElem = path.at(i-1);
				break;
			}
		}
		generateCheckPoints(maxPoints_,path);
}

void Route::generateCheckPoints(int maxPoints,vector<int> &path){
    checkPoints.push_back(path.at(0));
	for (int i = 1; i < path.size(); i++) {
        if (path.at(i) < maxPoints)
            checkPoints.push_back(path.at(i));
    }
	checkPoints.push_back(lastElem);
	
}

string Route::conflictString(){
	string s;
	set<int> keys;    
    for (auto it=conflict.begin(); it!=conflict.end(); ++it)
        keys.insert(it->first);
	for (int key : keys) { 
		   s += "routeConflicts[" + to_string(routeId) + ", " + to_string(key) +
		   		"] = " + (conflict.at(key) ? "true" : "false") + ",\n";
	}
	s = s.substr(0, s.length()-2);
	return s;
}

string Route::signalString(){
	string s = "routeSignal[" + to_string(routeId) + "] = { ";
	set<int> keys;    
    for (auto it=signals.begin(); it!=signals.end(); ++it){
        keys.insert(it->first);
	}
	for (int key : keys) {
		s += (signals.at(key) ? "true" : "false");
		s += ", ";
	}
	s = s.substr(0, s.length()-2);
	s += " }";	
	return s;
}

string Route::pointString(){
	string s = "routePoints[" + to_string(routeId) + "] = { ";
	
	set<int> keys;    
    for (auto it=points.begin(); it!=points.end(); ++it)
        keys.insert(it->first);
	for (int key : keys) { 
		s += points.at(key) + ", ";
	}
	s = s.substr(0, s.length() -2);
	s += " }";
	
	return s;
}
	
string Route::overlapString() {
	string s = "routeOverlap[" + to_string(routeId) + "] = { ";
	for ( int i = 0 ; i < overlap.size(); i++ ){
		s += (overlap.at(i) ? "true" : "false");
		s += ", ";
	}
	
	s = s.substr(0, s.length() - 2);
	s += " }";
			
	return s;
}

string Route::checkPointString(int max) {
	string s = "routeCheckPoints[" + to_string(routeId) + "] = { ";
	
	for ( int i = 0 ; i < checkPoints.size(); i++ )
		s += to_string(checkPoints.at(i)) + ", ";
	if (checkPoints.size() < max) {
			for ( int i = 0 ; i < max - checkPoints.size(); i++ )
				s +=  "-1, ";
	}
	s = s.substr(0, s.length() - 2);
	s += " }";		
	return s;
}

string Route::pathString(int max) {
	string s = "routePath[" + to_string(routeId) + "] = { ";	
	for ( int i = 0 ; i < path.size(); i++ )
		s += to_string(path.at(i)) + ", ";
	if (path.size() < max) {
			for ( int i = 0 ; i < max - path.size(); i++ )
				s +=  "-1, ";
	}
	s = s.substr(0, s.length() - 2);
	s += " }";			
	return s;
}

string Route::toString(int maxpath, int maxChunk){
	return "routeSrc[" + to_string(routeId) + "] = " + to_string(src)
			+ ",\nrouteDest[" + to_string(routeId) + "] = " + to_string(destination) 
			+ (!points.empty() ? ",\n" + pointString() : "")
			+ ",\n" + pathString(maxpath)
			+ ",\n" + signalString()
			+ ",\n" + overlapString()
			+ ",\n" + conflictString()
			+ ",\nrouteLastElem[" + to_string(routeId) +"] = " + to_string(lastElem) 
			+ ",\n" + checkPointString(maxChunk); 
}

