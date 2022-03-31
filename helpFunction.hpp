#pragma once
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"
#include "Route.hpp"
#include "NetworkLayout.hpp"
#include <string>


map<int,bool> signalsCombiner(map<int,bool> s1, map<int,bool> s2);
map<int,string> pointsCombiner(map<int,string> s1, map<int,string> s2);
vector<int> pathCombiner(vector<int> v1, vector<int> v2);
vector<bool> overlapCombiner(vector<bool> v1, vector<bool> v2);
map<int,bool> conflictCombiner(map<int,bool> m1, map<int,bool> m2);


map<int,bool> signalsCombiner(map<int,bool> s1, map<int,bool> s2){
    for(int i = 0; i < s1.size();i++){
        if(s1.at(i) != s2.at(i))
            s1.at(i) = s2.at(i);
    }
    return s1; 
}

map<int,string> pointsCombiner(map<int,string> s1, map<int,string> s2){
    for(int i = 0; i < s1.size();i++){
        if(s1.at(i) != s2.at(i))
            s1.at(i) = s2.at(i);
    }
    return s1;
}

vector<int> pathCombiner(vector<int> v1, vector<int> v2){
    vector<int> path;
    for(int i = 0; i < v1.size();i++){
        path.push_back(v1.at(i));
    }
    for(int i = 1; i < v2.size();i++){
        path.push_back(v2.at(i));
    }
    return path;
}

vector<bool> overlapCombiner(vector<bool> v1, vector<bool> v2){
    return v1;
}

map<int,bool> conflictCombiner(map<int,bool> m1,map<int,bool>m2){
    for(int i = 0; i < m2.size();i++){
        if(m1.at(i)== true and m2.at(i) == false)
            m2.at(i) == true;
    }
    return m2;
}