#pragma once

#include <string>
#include <map>
using namespace std;

class Signal{
private:
    int mbId;
    int sectionId;
	string linearEnd;
public:
    int getMbId(){return mbId;}
    int getSectionId(){return sectionId;}
    string getLinearEnd(){return linearEnd;}
    Signal(int &id, int &secId, string &linEnd);
    string toString();
    string toString(map<int,string>,map<int,string>);

};


