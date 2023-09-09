#pragma once
#include <map>

/*
Parent class of linear and point

*/
class Section {
public:
    int sectionId;
    explicit Section(int id);
    Section() = default;
    virtual ~Section() {}

    
};

