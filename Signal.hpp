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
    int getMbId() const{return mbId;}
    int getSectionId() const{return sectionId;}
    string getLinearEnd() const{return linearEnd;}
    Signal(const int &id,const int &secId,const string &linEnd);
    string toString() const;
    string toString(const map<int,string> &,const map<int,string> &) const;

};


