#include <string>
#include <iostream>
#include "Signal.hpp"

using namespace std;

Signal::Signal(int &id, int &secId, string &linEnd){
    mbId = id;
    sectionId = secId;
    linearEnd = linEnd;
    //cout << to_string(mbId) << " " <<  to_string(sectionId) << " "<< linearEnd<< endl;

};

string Signal::toString(){
    string mId = to_string(mbId);
    //cout << mId << " " <<  to_string(sectionId) << " "<< linearEnd<< endl;
    return "mbSection[" + mId + "] = " + to_string(sectionId)
            + ",\nmbLinearEnd[" + mId + "] = " + linearEnd;
}

string Signal::toString(map<int,string> plC,map<int,string> sC){
    string mId = sC.find(mbId)->second;
    //cout << mId << " " <<  to_string(sectionId) << " "<< linearEnd<< endl;
    return "mbSection[" + mId + " - " + to_string(mbId) + "] = " + plC.find(sectionId)->second + " - " + to_string(sectionId)
            + ",\nmbLinearEnd[" + mId + " - " + to_string(mbId) + "] = " + linearEnd;
}