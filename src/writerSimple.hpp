#pragma once
#include "writer.hpp"
#include <string>
#include <iostream>
#include <fstream>


using namespace std;


/*
Global overview of railway interlocking
*/

class writerSimple : public writer
{
public:
    writerSimple()= default;
    ~writerSimple() = default;
    void writeFile(string, unique_ptr<ParserXML>&,int ,int ,int , int) override;
    friend class writer;
};
