#pragma once
#include <string>
#include <map>

class Signal{
private:
    int mbId;
    int sectionId;
	std::string linearEnd;
public:
    int getMbId() const{return mbId;}
    int getSectionId() const{return sectionId;}
    std::string getLinearEnd() const{return linearEnd;}
    Signal(const int &id,const int &secId,const std::string &linEnd);
    std::string toString() const;
    std::string toString(const std::map<int,std::string> &,const std::map<int,std::string> &) const;
};

