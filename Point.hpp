#pragma once
#include "Section.hpp"
#include <string>

using namespace std;
class Point :  public Section{
private:
    int stemNeighbour;
    int plusNeighbour;
    int minusNeighbour;
public:
    Point(int &id,const int &stem,const int &plus,const int &minus) : Section(id), stemNeighbour(stem), plusNeighbour(plus), minusNeighbour(minus){};
    string toString(const map<int,string>&);
    string toString() const;
    int getStem() const{ return stemNeighbour; }
    int getPlus() const{ return plusNeighbour; }
    int getMinus() const{ return minusNeighbour; }
    int getSectionId() const{ return sectionId; }
    
};

