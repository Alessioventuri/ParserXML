#include "ParserXML.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <random>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;


int ParserXML::getIntFromNeighborLinear(xml_node<> *neighbor) const {
    auto neig = (string)neighbor->first_attribute("side")->value();
    if(neig == "up")
        return 0;
    else
        return 1;
}

int ParserXML::getIntFromNeighborPoint(xml_node<> *neighbor) const{
    auto neig = (string)neighbor->first_attribute("side")->value();
    if(neig == "stem")
        return 0;
    else if(neig == "plus")
        return 1;
    else return 2;
}

string ParserXML::SplitFilename(string str) const 
{
  std::size_t found = str.find_last_of("/\\");
  str = str.substr(found+1);
  return str.substr(0,str.length()-4);
}



ParserXML::ParserXML(const string in) : input(in){

    xml_document<> doc;
    xml_node<> *root_node;

    // Read the xml file into a vector
    ifstream inputfile (input);
    vector<char> buffer((istreambuf_iterator<char>(inputfile)), istreambuf_iterator<char>());
    buffer.push_back('\0');
    // Parse the buffer using the xml file parsing library into doc     
    doc.parse<0>(&buffer[0]);
    // Find our root node
    root_node = doc.first_node();
    // Iterate until reach the interlocking node
    while(((string)root_node->name()) != "interlocking"){
        if(root_node->first_node() == nullptr){
            root_node = root_node->next_sibling();
        } else{
            root_node = root_node->first_node();
        }
    }

    xml_node<> *network_node = root_node->first_node("network");
    
    searchPoints(network_node);
    searchLinears(network_node);
    searchSignals(network_node);
    NetworkLayoutProcess(network_node);
    InterlockingProcess(network_node);
}

int ParserXML::countRoutes(string input){
    xml_document<> doc;
    xml_node<> *root_node;

    // Read the xml file into a vector
    ifstream inputfile (input);
    vector<char> buffer((istreambuf_iterator<char>(inputfile)), istreambuf_iterator<char>());
    buffer.push_back('\0');
    // Parse the buffer using the xml file parsing library into doc     
    doc.parse<0>(&buffer[0]);
    // Find our root node
    root_node = doc.first_node();

    while(((string)root_node->name()) != "interlocking"){
        if(root_node->first_node() == nullptr){
            root_node = root_node->next_sibling();
        } else{
            root_node = root_node->first_node();
        }
    }
    xml_node<> *network_node = root_node->first_node("network");
    // cout << network_node->name() << endl;
    xml_node<> *routeTable = network_node->next_sibling();
    // cout << routeTable->name() << endl;
    int count = 0;
    for(xml_node<> *route = routeTable->first_node("route");route;route= route->next_sibling()){
        if((string)route->name() == "route"){
            count++;
        }
    }
    return count;
}

std::vector<std::vector<std::string>> ParserXML::get_vector_from_il(){
    std::vector<std::vector<std::string>> vectors;
    for( auto route : il.getRoutes()){
        std::vector<std::string> vector;
        for(auto element : route.getPath()){
            auto it = plCorrispondence.find(element);
            if (it != plCorrispondence.end()) {
                std::string a = it->second;
                vector.push_back(a);
            }
        }
        vectors.push_back(vector);
    }
    return vectors;
}

std::vector<std::vector<std::string>> ParserXML::get_signals_from_network(){
   
    std::vector<Linear> linear_vec = nl.getLinears();
    std::vector<Signal> signal_vec = nl.getSignals();

    std::vector<std::vector<std::string>> section_strings;
    for (auto& linear : linear_vec) {
        std::vector<std::string> lin_sig;
        lin_sig.push_back(plCorrispondence.find(linear.getSectionId())->second);
        for(auto& signal : signal_vec){
            if( signal.getSectionId() == linear.getSectionId()){
                lin_sig.push_back(mbCorrispondence.find(signal.getMbId())->second);
            }
        }
        section_strings.push_back(lin_sig);
    }
    return section_strings;
}

std::vector<std::vector<std::string>> ParserXML::get_vector_from_network(){
    std::vector<std::vector<std::string>> vectors;
    std::vector<std::unique_ptr<Section>> sections;

    // Add Linear objects to sections vector
    std::vector<Linear> linear_vec = nl.getLinears();
    for (auto& linear : linear_vec) {
        sections.push_back(std::make_unique<Linear>(linear));
        
    }

    // Add Point objects to sections vector
    std::vector<Point> point_vec = nl.getPoints();
    for (auto& point : point_vec) {
        sections.push_back(std::make_unique<Point>(point));
    }

    std::vector<std::vector<std::string>> section_strings;
    for (auto& section : sections) {
        if (auto point = dynamic_cast<Point*>(section.get())) {
            std::vector<std::string> points;
            points.push_back(plCorrispondence.find(point->getSectionId())->second);
            points.push_back(plCorrispondence.find(point->getStem())->second);
            points.push_back("S");
            section_strings.push_back(points);
            points.clear();
            points.push_back(plCorrispondence.find(point->getSectionId())->second);
            points.push_back(plCorrispondence.find(point->getPlus())->second);
            points.push_back("P");
            section_strings.push_back(points);
            points.clear();
            points.push_back(plCorrispondence.find(point->getSectionId())->second);
            points.push_back(plCorrispondence.find(point->getMinus())->second);
            points.push_back("M");
            section_strings.push_back(points);
        } else if (auto linear = dynamic_cast<Linear*>(section.get())) {
            std::vector<std::string> linears;
            if(linear->getDownNeig() != -1){
                linears.push_back(plCorrispondence.find(linear->getSectionId())->second);
                linears.push_back(plCorrispondence.find(linear->getDownNeig())->second);
                section_strings.push_back(linears);
            }
            linears.clear();
            if(linear->getUpNeig() != -1){
                linears.push_back(plCorrispondence.find(linear->getSectionId())->second);
                linears.push_back(plCorrispondence.find(linear->getUpNeig())->second);
                section_strings.push_back(linears);
            }
        }
    }
    // Delete the dynamically allocated objects in sections
    sections.clear();

    return section_strings;
}



void ParserXML::searchPoints(xml_node<> *network_node){
    for(auto trackSection =network_node->first_node("trackSection");trackSection;trackSection = trackSection->next_sibling()){
        if((string)trackSection->name() == "trackSection" && (string)trackSection->first_attribute("type")->value() == "point"){
            string value = trackSection->first_attribute("id")->value();
            this->id.insert(pair<string,int>(value,this->id.size()));
            this->plCorrispondence.insert(pair<int,string>(this->id.size()-1,"_"+value));
        }
    }
}

void ParserXML::searchLinears(xml_node<> *network_node){
    for(auto trackSection =network_node->first_node("trackSection");trackSection;trackSection = trackSection->next_sibling()){
        if((string)trackSection->name() == "trackSection" && (string)trackSection->first_attribute("type")->value() == "linear"){
            string value = trackSection->first_attribute("id")->value();
            this->id.insert(pair<string,int>(value,this->id.size()));
            this->plCorrispondence.insert(pair<int,string>(this->id.size()-1,"_"+value));
        }
    }
}

void ParserXML::searchSignals(xml_node<> *network_node){
    
    for(auto markerboard =network_node->first_node("markerboard");markerboard;markerboard = markerboard->next_sibling()){
        if((string)markerboard->name() == "markerboard"){
            string value = markerboard->first_attribute("id")->value();
            this->id.insert(pair<string,int>(value,count));
            this->mbCorrispondence.insert(pair<int,string>(count,("_"+value)));

            auto a =(this->id.find(markerboard->first_attribute("track")->value()))->second;
            auto b = (string)markerboard->first_attribute("mounted")->value(); 

            this->nl.addSignal(count,a,b);
            count++;
        }
    }
}
void ParserXML::NetworkLayoutProcess(xml_node<> *network_node){
    for(auto type =network_node->first_node("trackSection");type;type = type->next_sibling()){
        if((string)type->name() == "trackSection"){
            if((string)type->first_attribute("type")->value()=="point"){
                auto neighbor = type->first_node();
                int stem =-1,plus = -1,minus = -1;
                for(;neighbor;neighbor=neighbor->next_sibling()){
                    switch(this->getIntFromNeighborPoint(neighbor)){
                        case 0:
                            stem = this->id.find(neighbor->first_attribute("ref")->value())->second;
                            break;
                        case 1:
                            plus = this->id.find(neighbor->first_attribute("ref")->value())->second;
                            break;
                        default : 
                            minus = this->id.find(neighbor->first_attribute("ref")->value())->second;
                            break;
                    }
                }
                this->nl.addPoint(this->id.find(type->first_attribute("id")->value())->second,stem,plus,minus);
            }
            else{
                auto neighbor = type->first_node();
                int up = -1,down = -1;
                for(;neighbor;neighbor=neighbor->next_sibling()){
                    switch(this->getIntFromNeighborLinear(neighbor)){
                        case 0 : 
                            up = this->id.find(neighbor->first_attribute("ref")->value())->second;
                            break;
                        default : 
                            down = this->id.find(neighbor->first_attribute("ref")->value())->second;
                            break;
                    }
                }
                this->nl.addLinear(this->id.find(type->first_attribute("id")->value())->second,up,down); 
            }
        }
    }  
}



void ParserXML::InterlockingProcess(xml_node<> *network_node){
    xml_node<> *routeTable = network_node->next_sibling();
    int maxRoutes = 0;
    count = 0;
    for(xml_node<> *route = routeTable->first_node("route");route;route= route->next_sibling()){
        if((string)route->name() == "route"){
            this->id.insert(pair<string,int>((string)route->first_attribute("id")->value(),count));
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

        for(int i = 0; i < (int)this->nl.getPoints().size(); i++){
            points.insert(pair<int,string>(this->nl.getPoints().at(i).sectionId,"INTER"));
        }
        for(int i = 0;i < (int)this->nl.getSignals().size();i++){
            signals.insert(pair<int,bool>(this->nl.getSignals().at(i).getMbId(),false));

        }
        for(int i = 0;i < (int)(this->nl.getLinears().size() + this->nl.getPoints().size());i++){
            overlap.push_back(false);

        }
        for(int i = 0;i < maxRoutes; i++){
            conflict.insert(pair<int,bool>(i,false));
        } 

        //ADD SOURCE AND DESTINATION TO THE SIGNALS OF ROUTE
        signals.at(this->id.find(route->first_attribute("source")->value())->second) = true;
        signals.at(this->id.find(route->first_attribute("destination")->value())->second) = true;
        string direction;
        auto attr = route->first_attribute("dir");
        if (attr) {
            direction = attr->value();
        } 
        else {
            direction = "null";
        }
       //ADD THE SOURCE SECTION ON THE PATH ( IT'S COMING FROM THAT SECTION TO THE DESTINATION'S SECTION)
        
        path.push_back(nl.getSignals().at(this->id.find(route->first_attribute("source")->value())->second).getSectionId());

        int tempPath = 1;
        for(xml_node<> *icondition = route->first_node("condition");icondition ;icondition= icondition->next_sibling()){  
            if((string)icondition->name() == "condition" ){
                if((string)icondition->first_attribute("type")->value() == "point"){
                    auto stringpoint = (string)icondition->first_attribute("val")->value();
                    for(int x = 0; x < (int)stringpoint.length();x++){
                        stringpoint[x] = stringpoint[x] - 32;
                    }
                    points.at(this->id.find(icondition->first_attribute("ref")->value())->second) = stringpoint;
                }
                else if((string)icondition->first_attribute("type")->value() == "signal"){
                    signals.at(this->id.find(icondition->first_attribute("ref")->value())->second) = true;
                }
                else if((string)icondition->first_attribute("type")->value() == "trackvacancy"){
                    path.push_back(this->id.find(icondition->first_attribute("ref")->value())->second);
                    tempPath++;
                }
                else {
                    conflict.at(this->id.find(icondition->first_attribute("ref")->value())->second) = true;
                }
            }
        }

        if(tempPath > maxPathLenght)
            maxPathLenght = tempPath;
        auto size = (int)this->nl.getPoints().size();
        Route rou(this->id.find(route->first_attribute("id")->value())->second,
            this->id.find(route->first_attribute("source")->value())->second,
            this->id.find(route->first_attribute("destination")->value())->second,
            direction,
            points,
            path,
            signals,
            overlap,
            conflict,
            size);
        this->il.addRoute(rou);
        this->il.setMaxValues(maxPathLenght);
    }

}

int ParserXML::getSecondRoute(int index){
    bool found = false;
    std::uniform_int_distribution<int> distribution(0, (int)this->il.getRoutes().at(index).getConflict().size()-1);
    std::random_device rd;
    std::mt19937 engine(rd());
    while(!found){
        secondRoute = distribution(engine);
        if(this->il.getRoutes().at(index).getConflict().at(secondRoute) == false && index != secondRoute){
            found = true;
        }
    }
    return secondRoute;
}