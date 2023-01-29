#pragma once
#include "Section.hpp"
#include <string>

class Linear :  public Section {
	
private:
    int upNeighbour;
	int downNeighbour;

public: 
    Linear(int &id,const int &up,const int &down) : Section(id){
		upNeighbour = up;
		downNeighbour = down;  
	}
	std::string toString(const std::map<int,std::string>&) const;
    std::string toString() const;
	int getUpNeig() const{return upNeighbour;}
	int getDownNeig() const{return downNeighbour;}
	int getSectionId() const{ return sectionId;}

};
