#pragma once
#include "Section.hpp"
#include <string>
using namespace std;

class Linear :  public Section {
	
private:
    int upNeighbour;
	int downNeighbour;

public: 
    Linear(int &id, int &up, int &down) : Section(id){
		upNeighbour = up;
		downNeighbour = down;
        
	}
	string toString(map<int,string>);
    string toString();
	int getUpNeig(){return upNeighbour;}
	int getDownNeig(){return downNeighbour;}
};
