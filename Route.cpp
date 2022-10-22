#include "Route.hpp"


Route::Route(const int &id_,int src_, int dest_,const string &dire_, map<int,string> const &points_,const vector<int> &path_,
 	map<int,bool>const & signals_, vector<bool>const & overlap_, map<int,bool>const & conflict_, int maxPoints_) : 
	routeId(id_), src(src_), direction(dire_), points(points_),path(path_), signals(signals_),overlap(overlap_),conflict(conflict_)
	{
		lastElem = path.at(path.size() -1);

		for (int i = 0; i < (int)path.size(); i++) {
			if (path.at(i) == -1){
				lastElem = path.at(i-1);
				break;
			}
		}
		generateCheckPoints(maxPoints_,path);
}

void Route::generateCheckPoints(int maxPoints,vector<int> &paths){
    checkPoints.push_back(paths.at(0));
	for (int i = 1; i < (int)paths.size(); i++) {
        if (paths.at(i) < maxPoints)
            checkPoints.push_back(paths.at(i));
    }
	checkPoints.push_back(lastElem);
	
}

string Route::conflictString(){
	string s;
	set<int> keys;    
	for(const auto& [key1, value] : conflict)
        keys.insert(key1);
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
	for(const auto& [key, value] :signals)  
        keys.insert(key);
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
	for(auto point : points)
        keys.insert(point.first);
	for (int key : keys) { 
		s += points.at(key) + ", ";
	}
	s = s.substr(0, s.length() -2);
	s += " }";
	
	return s;
}
	
string Route::overlapString() {
	string s = "routeOverlap[" + to_string(routeId) + "] = { ";
	for(bool over : overlap){
		s += over ? "true" : "false";
		s += ", ";
	}
	s = s.substr(0, s.length() - 2);
	s += " }";
			
	return s;
}

string Route::checkPointString(int max) {
	string s = "routeCheckPoints[" + to_string(routeId) + "] = { ";
	s += createString(path,max);
	return s;
}


string Route::pathString(int max) {
	string s = "routePath[" + to_string(routeId) + "] = { ";	
	s += createString(path,max);	
	return s;
}

string Route::toString(int maxpath, int maxChunk){
	return "routeSrc[" + to_string(routeId) + "] = " + to_string(src)
			+ "\nrouteDirection[" + to_string(routeId) + "] = " + getDirection() 
			+ ",\nrouteDest[" + to_string(routeId) + "] = " + to_string(destination) 
			+ (!points.empty() ? ",\n" + pointString() : "")
			+ ",\n" + pathString(maxpath)
			+ ",\n" + signalString()
			+ ",\n" + overlapString()
			+ ",\n" + conflictString()
			+ ",\nrouteLastElem[" + to_string(routeId) +"] = " + to_string(lastElem) 
			+ ",\n" + checkPointString(maxChunk); 
}
// string Route::toString(int maxpath){
// 	return "routeSrc[" + to_string(routeId) + "] = " + to_string(src)
// 			+ "\nrouteDirection[" + to_string(routeId) + "] = " + getDirection() 
// 			+ ",\nrouteDest[" + to_string(routeId) + "] = " + to_string(destination) 
// 			+ (!points.empty() ? ",\n" + pointString() : "")
// 			+ ",\n" + pathString(maxpath)
// 			+ ",\n" + signalString()
// 			+ ",\n" + overlapString()
// 			+ ",\n" + conflictString()
// 			+ ",\nrouteLastElem[" + to_string(routeId) +"] = " + to_string(lastElem) 
// 			+ ",\n";
// }

string Route::createString(vector<int> a,int max){
	int i = 0;
	string s;
	for(auto it = a.begin(); it != a.end();++it){
		s += to_string(a.at(i)) + ", ";
		i++;
	}
	if ((int)a.size() < max) {
		for ( int j = 0 ; j < max - (int)a.size(); j++ )
			s +=  "-1, ";
	}
	s = s.substr(0, s.length() - 2);
	s += " }";
	return s;
}

