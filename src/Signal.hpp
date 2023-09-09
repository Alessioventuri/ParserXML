#pragma once
#include <string>
#include <map>

/*
Signal element 
It is associated to only one linear
*/
class Signal{
private:
    int mbId;
    int sectionId;
	std::string direction;
public:
    int getMbId() const{return mbId;}
    int getSectionId() const{return sectionId;}
    std::string getLinearEnd() const{return direction;}
    Signal(const int &id,const int &secId,const std::string &linEnd);
    std::string toString(const std::map<int,std::string> &,const std::map<int,std::string> &) const;
};

