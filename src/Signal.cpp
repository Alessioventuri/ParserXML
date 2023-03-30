#include <string>
#include <iostream>
#include "Signal.hpp"

using namespace std;

Signal::Signal(const int &id,const int &secId,const string &linEnd) : mbId(id), sectionId(secId), direction(linEnd){
};

string Signal::toString(const map<int,string> &plC,const map<int,string> &sC) const{
    string mId = sC.find(mbId)->second;
    return "mbSection[" + mId + " - " + to_string(mbId) + "] = " + plC.find(sectionId)->second + " - " + to_string(sectionId)
            + ",\nmbLinearEnd[" + mId + " - " + to_string(mbId) + "] = " + direction;
}