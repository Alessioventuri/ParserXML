#include "Point.hpp"

using namespace std;


string Point::toString(const map<int,string> &plC){
    
    string id = plC.find(sectionId)->second;
    return "pointStem[" + id + " - " + to_string(getSectionId()) +"] = " + plC.find(stemNeighbour)->second + " - "+ to_string(stemNeighbour)
            + ",\npointPlus[" + id + " - " + to_string(getSectionId())+"] = " + plC.find(plusNeighbour)->second + " - " + to_string(plusNeighbour)
            + ",\npointMinus[" + id + " - " +to_string(getSectionId())+ "] = " + plC.find(minusNeighbour)->second + " - " + to_string(minusNeighbour);
}