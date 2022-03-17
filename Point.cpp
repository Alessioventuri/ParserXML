#include "Point.hpp"

using namespace std;

string Point::toString(){
    string secId = to_string(sectionId);
    return "pointStem[" + secId + "] = " + to_string(stemNeighbour)
            + ",\npointPlus[" + secId + "] = " + to_string(plusNeighbour) 
            + ",\npointMinus[" + secId + "] = " + to_string(minusNeighbour);
}
string Point::toString(map<int,string> pl){
    
    string id = pl.find(sectionId)->second;
    return "pointStem[" + id + " - " + to_string(getSectionId()) +"] = " + pl.find(stemNeighbour)->second + " - "+ to_string(stemNeighbour)
            + ",\npointPlus[" + id + " - " + to_string(getSectionId())+"] = " + pl.find(plusNeighbour)->second + " - " + to_string(plusNeighbour)
            + ",\npointMinus[" + id + " - " +to_string(getSectionId())+ "] = " + pl.find(minusNeighbour)->second + " - " + to_string(minusNeighbour);
}