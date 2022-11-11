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
	std::string toString(std::map<int,std::string>);
    std::string toString();
	int getUpNeig() {return upNeighbour;}
	int getDownNeig(){return downNeighbour;}
};
