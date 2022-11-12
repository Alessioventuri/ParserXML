
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"
#include "NetworkLayout.hpp"
#include <string>


std::map<int,bool> signalsCombiner(std::map<int,bool> s1, std::map<int,bool> s2);
std::map<int,std::string> pointsCombiner(std::map<int,std::string> s1, std::map<int,std::string> s2);
std::vector<int> pathCombiner(std::vector<int> v1, std::vector<int> v2);
std::vector<bool> overlapCombiner(std::vector<bool> v1, std::vector<bool> &v2);
std::map<int,bool> conflictCombiner(std::map<int,bool> m1, std::map<int,bool> m2);


std::map<int,bool> signalsCombiner(std::map<int,bool> s1, std::map<int,bool> s2){
    for(int i = 0; i < s1.size();i++){
        if(s1.at(i) == false and s2.at(i) == true)
            s1.at(i) = s2.at(i);
    }
    return s1; 
}
//s1.at(i) == "INTER" and s2.at(i) != "INTER" or not because the s2 is supposed to be bigger that s1.
std::map<int,std::string> pointsCombiner(std::map<int,std::string> s1, std::map<int,std::string> s2){
    for(int i = 0; i < s1.size();i++){
        if(s1.at(i) != s2.at(i))
            s1.at(i) = s2.at(i);
    }
    return s1;
}


std::vector<int> pathCombiner(std::vector<int> v1, std::vector<int> v2){
    std::vector<int> path;
    for(int i = 0; i < v1.size();i++){
        path.push_back(v1.at(i));
    }
    for(int i = 1; i < v2.size();i++){
        path.push_back(v2.at(i));
    }
    return path;
}

std::vector<bool> overlapCombiner(std::vector<bool> v1, std::vector<bool> &v2){
    return v1;
}

std::map<int,bool> conflictCombiner(std::map<int,bool> m1,std::map<int,bool>m2){
    for(int i = 0; i < m2.size();i++){
        if(m1.at(i)== true and m2.at(i) == false)
            m2.at(i) = true;
    }
    return m2;
}