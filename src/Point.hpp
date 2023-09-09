#pragma once
#include "Section.hpp"
#include <string>

/*
This represent the Point element of a railway interlocking
stem -> plus : main branch
stem -> minus : deviation branch
*/

class Point :  public Section{
private:
    int stemNeighbour;
    int plusNeighbour;
    int minusNeighbour;
public:
    Point(const int &id,const int &stem,const int &plus,const int &minus) : Section(id), stemNeighbour(stem), plusNeighbour(plus), minusNeighbour(minus){};
    std::string toString(const std::map<int,std::string>&);
    int getStem() const{ return stemNeighbour; }
    int getPlus() const{ return plusNeighbour; }
    int getMinus() const{ return minusNeighbour; }
    int getSectionId() const{ return sectionId; }
    
};

