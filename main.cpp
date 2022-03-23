#include "Interlock.hpp"
#include "NetworkLayout.hpp"
#include "writer.hpp"
#include "writerUMC.hpp"
#include "writerSimple.hpp"
#include <stdexcept>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"
#include <unordered_map>
#include <string>
#include <cstring>
#include <exception>

using namespace rapidxml;
using namespace std;

int getIntFromNeighborLinear(xml_node<> *neighbor){
    string neig = (string)neighbor->first_attribute("side")->value();
    if(neig == "up")
        return 0;
    else
        return 1;
}

int getIntFromNeighborPoint(xml_node<> *neighbor){
    string neig = (string)neighbor->first_attribute("side")->value();
    if(neig == "stem")
        return 0;
    else if(neig == "plus")
        return 1;
    else return 2;
}

writer* writer::write(fileType type){
    if(type == fileType::UMCFile) 
        return new writerUMC();
    if(type == fileType::SimpleFile)
        return new writerSimple();
    return NULL;
}

int main(int argc, char *argv[]){
    bool outputToFile = false;
    string input;
    string outputFile = "/Users/alessioventuri/Desktop/Interlocking/";
    bool helpCalled = false;
    
    // for(int i = 0; i < argc; i++){
    //     string stringInput = argv[i];
    //     if(stringInput == ("-i")){
    //         if(argc <= i+1 && !helpCalled) 
    //             throw invalid_argument("There must be an input file");
    //         input = argv[i+1];
    //     }
    //     if(stringInput == "-o"){
    //         if(argc >i+1){
    //             outputFile = argv[i+1];
    //             outputToFile = true;
    //         }
    //     }
    //     if(stringInput == "-h"){
    //         helpCalled = true;
    //         break;
    //     }
    // }
    // if(input == "" && !helpCalled)
    //     throw invalid_argument("There must be an input file");
    // if (helpCalled){
    //     cout << "-i <inputfile> ";
    //     cout << "Provides the input xml file that should be parsed" << endl;
    //     cout << "-o <outputfile> ";
    //     cout << "Provides the output text which the program should write the config data"<<endl;
    //     cout << "This remove the printout in the terminal and is optional"<< endl;
    //     cout << "-h ";
    //     cout << "Shows this info" << endl;

    //     exit(0);

    // }
    cout << "Parsing the xml : " <<  input << endl;

    xml_document<> doc;
    xml_node<> *root_node;

    // Read the xml file into a vector
    ifstream inputfile ("/Users/alessioventuri/Desktop/XML/lvr_7_right_rt.xml");
    vector<char> buffer((istreambuf_iterator<char>(inputfile)), istreambuf_iterator<char>());

    // Parse the buffer using the xml file parsing library into doc     
    doc.parse<0>(&buffer[0]);

    // Find our root node
    root_node = doc.first_node();
    
    // Iterate until reach the interlocking node
    std::cout << "Name of my first node is: " << doc.first_node()->name() << "\n";
    while(((string)root_node->name()) != "interlocking"){
        if(root_node->first_node() == nullptr){
            root_node = root_node->next_sibling();
            std::cout << "Name of my first node sibling is: " << root_node->name() << "\n";
        } else{
            root_node = root_node->first_node();
            std::cout << "Name of my first node is: " << root_node->name() << "\n";
        }
    }
    cout << "Sono arrivato a :" << root_node->name()<<endl;
   	unordered_map<string, int> id;
	NetworkLayout nl;
	Interlock il ;
    xml_node<> *network_node = root_node->first_node("network");
    xml_node<> *trackSection = network_node->first_node("trackSection");

    cout << "Sono arrivato a :" << network_node->name()<<endl;
    cout << "Sono arrivato a :" << trackSection->name()<<endl;

    int count = 0;
    // cout << typeid(trackSection =network_node->first_node("trackSection")).name() << endl;
    map<int,string> plCorrispondence;
    map<int,string> mbCorrispondence;

    //CHECK TRACKSECTION POINT AND ADD HIS ID TO MAPid
    for(auto trackSection =network_node->first_node("trackSection");trackSection;trackSection = trackSection->next_sibling()){
        if((string)trackSection->name() == "trackSection"){
            if((string)trackSection->first_attribute("type")->value() == "point"){
                id.insert(pair<string,int>(trackSection->first_attribute("id")->value(),id.size()));
                plCorrispondence.insert(pair<int,string>(id.size()-1,trackSection->first_attribute("id")->value()));
               // cout << trackSection->first_attribute("id")->value()<<endl;
            }
        }
    }
    //CHECK TRACKSECTION LINEAR AND ADD HIS ID TO MAPid
    for(auto trackSection =network_node->first_node("trackSection");trackSection;trackSection = trackSection->next_sibling()){
        if((string)trackSection->name() == "trackSection"){
            if((string)trackSection->first_attribute("type")->value() == "linear"){
                id.insert(pair<string,int>(trackSection->first_attribute("id")->value(),id.size()));
                plCorrispondence.insert(pair<int,string>(id.size()-1,trackSection->first_attribute("id")->value()));

              //  cout << trackSection->first_attribute("id")->value()<<endl;
            }
        }
    }
    cout << "dimensione mappa " <<id.size()<<endl;

    //CHECK MARKERBOARD AND ADD TO NETWORKLAYOUT SIGNALS
    for(auto markerboard =network_node->first_node("markerboard");markerboard;markerboard = markerboard->next_sibling()){
        if((string)markerboard->name() == "markerboard"){
            id.insert(pair<string,int>(markerboard->first_attribute("id")->value(),count));
            mbCorrispondence.insert(pair<int,string>(count,markerboard->first_attribute("id")->value()));

            auto a =(id.find(markerboard->first_attribute("track")->value()))->second;
            auto b = (string)markerboard->first_attribute("mounted")->value(); 

            //UPPER CASE
            // for(int x = 0; x < b.length();x++){
            //     b[x] = b[x] - 32;
            // }
            nl.addSignal(count,a,b);
            count++;
        }
    }

    cout << nl.getSignals().size() << " dimensione markerboard" << endl;
    cout << id.size() << " dimensione mappa" << endl;

    for(auto type =network_node->first_node("trackSection");type;type = type->next_sibling()){
        if((string)type->name() == "trackSection"){
            if((string)type->first_attribute("type")->value()=="point"){
                auto neighbor = type->first_node();
                int stem =-1,plus = -1,minus = -1;
                for(;neighbor;neighbor=neighbor->next_sibling()){
                    switch(getIntFromNeighborPoint(neighbor)){
                        case 0:
                            stem = id.find(neighbor->first_attribute("ref")->value())->second;
                            break;
                        case 1:
                            plus = id.find(neighbor->first_attribute("ref")->value())->second;
                            break;
                        default : 
                            minus = id.find(neighbor->first_attribute("ref")->value())->second;
                            break;
                    }
                }
                nl.addPoint(id.find(type->first_attribute("id")->value())->second,stem,plus,minus);
            }
            else{
                auto neighbor = type->first_node();
                int up = -1,down = -1;
                for(;neighbor;neighbor=neighbor->next_sibling()){
                    switch(getIntFromNeighborLinear(neighbor)){
                        case 0 : 
                            up = id.find(neighbor->first_attribute("ref")->value())->second;
                            break;
                        default : 
                            down = id.find(neighbor->first_attribute("ref")->value())->second;
                            break;
                    }
                }
                nl.addLinear(id.find(type->first_attribute("id")->value())->second,up,down); 
            }
        }
    }
    cout << nl.getLinears().size() << " e " << nl.getPoints().size();
    cout << "\n"<<endl;

    xml_node<> *routeTable = network_node->next_sibling();
    xml_node<> *routes = routeTable->first_node("route");
    xml_node<> *condition = routes->first_node("condition");
    cout << "Sono arrivato a :" << routeTable->name()<<endl;   
    cout << "Sono arrivato a :" << routes->name()<<endl;
    cout << "Sono arrivato a :" << condition->name()<<endl;
    int maxRoutes = 0;
    count = 0;
    for(xml_node<> *route = routeTable->first_node("route");route;route= route->next_sibling()){
        if((string)route->name() == "route"){
            id.insert(pair<string,int>((string)route->first_attribute("id")->value(),count));
            count++;
            maxRoutes++;
        }
    }
    int maxPathLenght = 0;
    for(xml_node<> *route = routeTable->first_node("route");route;route= route->next_sibling()){
        map<int,bool> signals;
        vector<int> path;
        map<int,string> points;
        map<int,bool> conflict;
        vector<bool> overlap;

        for(int i = 0; i < nl.getPoints().size(); i++){
            points.insert(pair<int,string>(nl.getPoints().at(i).sectionId,"INTER"));
        }
        for(int i = 0;i < nl.getSignals().size();i++){
            signals.insert(pair<int,bool>(nl.getSignals().at(i).getMbId(),false));

        }
        for(int i = 0;i < nl.getLinears().size()+ nl.getPoints().size();i++){
            overlap.push_back(false);

        }
        for(int i = 0;i <maxRoutes;i++){
            conflict.insert(pair<int,bool>(i,false));
        }

        //ADD SOURCE AND DESTINATION TO THE SIGNALS OF ROUTE
        signals.at(id.find(route->first_attribute("source")->value())->second) = true;
        signals.at(id.find(route->first_attribute("destination")->value())->second) = true;
        string direction = route->first_attribute("dir")->value();
        //ADD THE SOURCE SECTION ON THE PATH ( IT'S COMING FROM THAT SECTION TO THE DESTINATION'S SECTION)
        path.push_back(nl.getSignals().at(id.find(route->first_attribute("source")->value())->second).getSectionId());
        int tempPath = 1;
        for(xml_node<> *condition = route->first_node("condition");condition ;condition= condition->next_sibling()){  
            if((string)condition->name() == "condition"){
                if((string)condition->first_attribute("type")->value() == "point"){
                    auto stringpoint = (string)condition->first_attribute("val")->value();
                    for(int x = 0; x < stringpoint.length();x++){
                        stringpoint[x] = stringpoint[x] - 32;
                    }
                    points.at(id.find(condition->first_attribute("ref")->value())->second) = stringpoint;
                }
                else if((string)condition->first_attribute("type")->value() == "signal"){
                    signals.at(id.find(condition->first_attribute("ref")->value())->second) = true;
                }
                else if((string)condition->first_attribute("type")->value() == "trackvacancy"){
                    path.push_back(id.find(condition->first_attribute("ref")->value())->second);
                    tempPath++;
                }
                else {
                    conflict.at(id.find(condition->first_attribute("ref")->value())->second) = true;
                }
            }
        }
        if(tempPath > maxPathLenght)
            maxPathLenght = tempPath;
        int size = nl.getPoints().size();
        Route rou(id.find(route->first_attribute("id")->value())->second,
            id.find(route->first_attribute("source")->value())->second,
            id.find(route->first_attribute("destination")->value())->second,
            direction,
            points,
            path,
            signals,
            overlap,
            conflict,
            size);
        il.addRoute(rou);
        il.setMaxValues(maxPathLenght);

    }
    writer* obj = writer::write(SimpleFile);
    obj->writeFile(outputFile,nl,il,plCorrispondence,mbCorrispondence);
    delete obj;
}

