#include "writerUMC.hpp"
#include "ParserXML.hpp"
#include <string>
#include <ctype.h>
#include <algorithm>
//FIXME: Find the error



//void writerUMC::writeFile(string outputFile,NetworkLayout nl,Interlock il,map<int,string> pl,map<int,string> mb,int choose){
void writerUMC::writeFile(const string outputFile, ParserXML *pXML,int train,int select,int route1, int route2){
    // 1. create a file.txt for each route and add an extra routes that continue
    // 2. create a file.txt that contents all routes
    // 3. create a file.txt that contents some routes
    pXML->getIl().generateMaxChunk();
    if(select != 1){
        for ( int firstRoute = 0 ; firstRoute < (int)pXML->getIl().getRoutes().size(); firstRoute++ ){
            if(train == 1){
                if(outputFile != ""){ 
                    string outputFiletxt = outputFile + "route" + to_string(pXML->getIl().getRoutes().at(firstRoute).getRouteId()) + ".txt";      
                    try{
                        ofstream myfile;
                        myfile.open(outputFiletxt);
                        myfile << "\n/* NetworkLayout */\n\n";
                        myfile << pXML->getNl().toStringAdaptive(pXML->getIl().getRoutes().at(firstRoute),pXML->getPlCorrispondence(),pXML->getMbCorrispondence());
                        myfile << "\n\n/* NetworkLayout End */\n\n";
                        myfile << "\n/* Interlocking */\n\n";
                        myfile << pXML->getIl().getRoutes().at(firstRoute).toString(pXML->getIl().getMaxPathLength(), pXML->getIl().getMaxChunk())  + "\n";
                        myfile << "\n/* Interlocking End */\n";
                        myfile << "\n/* UMC code */\n"; 
                        //myfile << defaultUMCsetupOneRoute(pXML->getNl(),pXML->getIl(),i,pXML->getPlCorrispondence(),pXML->getMbCorrispondence());
                        myfile << defaultUMCsetupOneRoute(pXML,firstRoute);
                        myfile.close();
                        cout << "Successfully wrote to the file."<<endl;
                    }catch(const exception& e){
                        cerr << "An error occured" << endl;
                    }
                }
            }else{
                if(outputFile != ""){ 
                    int secondRoute = pXML->getSecondRoute(firstRoute);
                    string outputFiletxt = outputFile + "route" + stringCombinerId(pXML,firstRoute,secondRoute) + ".txt";      
                    try{
                        ofstream myfile;
                        myfile.open(outputFiletxt);
                        myfile << "\n/* NetworkLayout */\n\n";
                        myfile << stringCombinerNl(pXML,firstRoute,secondRoute);
                        myfile << "\n\n/* NetworkLayout End */\n\n";
                        myfile << "\n/* Interlocking */\n\n";
                        myfile << stringCombinerIl(pXML,firstRoute, secondRoute)  + "\n";
                        myfile << "\n/* Interlocking End */\n";
                        myfile << "\n/* UMC code */\n"; 
                        //myfile << defaultUMCsetupOneRoute(pXML->getNl(),pXML->getIl(),i,pXML->getPlCorrispondence(),pXML->getMbCorrispondence());
                         myfile << defaultUMCsetupTwoRoute(pXML,firstRoute,secondRoute);

                        myfile.close();
                        cout << "Successfully wrote to the file."<<endl;
                    }catch(const exception& e){
                        cerr << "An error occured" << endl;
                    }
                }
            }
        }
    }else{
        if(train == 1){
            if(outputFile != ""){  //check if you want print at display  
                string outputFiletxt = outputFile + "route" + to_string(pXML->getIl().getRoutes().at(route1).getRouteId()) + ".txt";      
                try{
                    ofstream myfile;
                    myfile.open(outputFiletxt);
                    myfile << "\n/* NetworkLayout */\n\n";
                    myfile << pXML->getNl().toStringAdaptive(pXML->getIl().getRoutes().at(route1),pXML->getPlCorrispondence(),pXML->getMbCorrispondence());
                    myfile << "\n\n/* NetworkLayout End */\n\n";
                    myfile << "\n/* Interlocking */\n\n";
                    myfile << pXML->getIl().getRoutes().at(route1).toString(pXML->getIl().getMaxPathLength(), pXML->getIl().getMaxChunk())  + "\n";
                    myfile << "\n/* Interlocking End */\n";
                    myfile << "\n/* UMC code */\n"; 
                    //myfile << defaultUMCsetupOneRoute(pXML->getNl(),pXML->getIl(),i,pXML->getPlCorrispondence(),pXML->getMbCorrispondence());
                    myfile << defaultUMCsetupOneRoute(pXML,route1);
                    myfile.close();
                    cout << "Successfully wrote to the file."<<endl;
                }catch(const exception& e){
                    cerr << "An error occured" << endl;
                }
            }
        }else{
            if(outputFile != ""){ 
                string outputFiletxt = outputFile + "route" + stringCombinerId(pXML,route1,route2) + ".txt";      
                try{
                    ofstream myfile;
                    myfile.open(outputFiletxt);
                    myfile << "\n/* NetworkLayout */\n\n";
                    myfile << stringCombinerNl(pXML,route1,route2);
                    myfile << "\n\n/* NetworkLayout End */\n\n";
                    myfile << "\n/* Interlocking */\n\n";
                    myfile << stringCombinerIl(pXML,route1, route2)  + "\n";
                    myfile << "\n/* Interlocking End */\n";
                    myfile << "\n/* UMC code */\n"; 
                    //myfile << defaultUMCsetupOneRoute(pXML->getNl(),pXML->getIl(),i,pXML->getPlCorrispondence(),pXML->getMbCorrispondence());
                    myfile << defaultUMCsetupTwoRoute(pXML,route1,route2);

                    myfile.close();
                    cout << "Successfully wrote to the file."<<endl;
                }catch(const exception& e){
                    cerr << "An error occured" << endl;
                }
            }
        }
    }
}

string writerUMC::defaultUMCsetupOneRoute(ParserXML *pXML, int firstRoute){
    string s;
    s += "Objects:\n\n";
    // In UMC I can't insert object that I didn't declare. So:
    // 1 - I use only the pathChunck
    // 2 - I declare everything (it may be too big)
    s += signalObjectUmcOneRoute(pXML->getIl().getRoutes().at(firstRoute), pXML->getPlCorrispondence(), pXML->getMbCorrispondence(), pXML->getNl());
    s += pointObjectUmcOneRoute (pXML->getIl().getRoutes().at(firstRoute), pXML->getPlCorrispondence(), pXML->getNl());  // changed
    s += linearObjectUmcOneRoute(pXML->getIl().getRoutes().at(firstRoute), 1, pXML->getPlCorrispondence(), pXML->getMbCorrispondence(), pXML->getNl());
    s += trainObjectUmcOneRoute (pXML->getIl().getRoutes().at(firstRoute), 1, pXML->getPlCorrispondence());
    s += abstractionUmcOneRoute (pXML->getIl().getRoutes().at(firstRoute), 1, pXML->getPlCorrispondence(), pXML->getMbCorrispondence(), pXML->getNl());
    return s;
}

string writerUMC::defaultUMCsetupTwoRoute(ParserXML *pXML, int firstRoute, int secondRoute){
    string s;
    s += "Objects:\n\n";
    s += signalObjectUmcOneRoute(pXML->getIl().getRoutes().at(firstRoute), pXML->getPlCorrispondence(), pXML->getMbCorrispondence(), pXML->getNl());
    s += signalObjectUmcOneRoute(pXML->getIl().getRoutes().at(secondRoute), pXML->getPlCorrispondence(), pXML->getMbCorrispondence(), pXML->getNl());

    // s += pointObjectUmcOneRoute (pXML->getIl().getRoutes().at(firstRoute), pXML->getPlCorrispondence(), pXML->getNl());  // changed
    // s += pointObjectUmcOneRoute (pXML->getIl().getRoutes().at(secondRoute), pXML->getPlCorrispondence(), pXML->getNl());  // changed

    s += pointObjectUmcTwoRoute (pXML->getIl().getRoutes().at(firstRoute), pXML->getIl().getRoutes().at(secondRoute), pXML->getPlCorrispondence(),pXML->getNl());

    s += linearObjectUmcOneRoute(pXML->getIl().getRoutes().at(firstRoute), 1, pXML->getPlCorrispondence(), pXML->getMbCorrispondence(), pXML->getNl());
    s += linearObjectUmcOneRoute(pXML->getIl().getRoutes().at(secondRoute), 2, pXML->getPlCorrispondence(), pXML->getMbCorrispondence(), pXML->getNl());
    
    s += trainObjectUmcOneRoute (pXML->getIl().getRoutes().at(firstRoute), 1, pXML->getPlCorrispondence());
    s += trainObjectUmcOneRoute (pXML->getIl().getRoutes().at(secondRoute), 2, pXML->getPlCorrispondence());

    s += abstractionUmcTwoRoute (pXML->getIl().getRoutes().at(firstRoute), pXML->getIl().getRoutes().at(secondRoute), 1, 2, pXML->getPlCorrispondence(), pXML->getMbCorrispondence(), pXML->getNl());
    return s;
}

string writerUMC::linearObjectUmcOneRoute(Route route,int number, const map<int,string> plC, const map<int,string> sC, NetworkLayout nl){
    string output;
    int index;
    for(int i = 0; i < (int)route.getPath().size();i++){
        if(route.getPath().at(i) >= (int)route.getPoints().size() and i != (int)route.getPath().size()-1){                                     
            index = route.getPath().at(i)-nl.getPoints().size(); 
            // I did that to put NULL on last cdb and stop the request. I know that there is also the point that have to checked
            string up  = nl.getLinears().at(index).getUpNeig() != -1  ? plC.find(nl.getLinears().at(index).getUpNeig())->second : "null";
            string down = nl.getLinears().at(index).getDownNeig() != -1  ? plC.find(nl.getLinears().at(index).getDownNeig())->second : "null";
            string sign = findMb(route,nl,nl.getLinears().at(index).sectionId,sC);
            string train = (nl.getLinears().at(index).sectionId == route.getPath().at(0)) ? "train"+to_string(number) : "null";
            output += plC.find(nl.getLinears().at(index).sectionId)->second + ": CircuitoDiBinario (\n\t";
            output += "prev => [";
            output += i != 0 ? (route.getDirection() == "up" ? down : up) : "null";
            output += "],\n\t";
            output += "next => [" + (route.getDirection() == "up" ? up : down) + "],\n\t";
            output += "sign => [" + sign + "],\n\t";
            output += "treno => " + train +"\n);\n\n";
        }else if(i == (int)route.getPath().size()-1){
            index = route.getPath().at(i)-nl.getPoints().size(); 
            string up  = nl.getLinears().at(index).getUpNeig() != -1  ? plC.find(nl.getLinears().at(index).getUpNeig())->second : "null";
            string down = nl.getLinears().at(index).getDownNeig() != -1  ? plC.find(nl.getLinears().at(index).getDownNeig())->second : "null";
            string sign = findMb(route,nl,nl.getLinears().at(index).sectionId,sC);
            string train = (nl.getLinears().at(index).sectionId == route.getPath().at(0)) ? "train"+number : "null";
            output += plC.find(nl.getLinears().at(index).sectionId)->second + ": CircuitoDiBinario (\n\t";
            output += "prev => [" + (route.getDirection() == "up" ? down : up) + "],\n\t";
            output += "next => [null],\n\t";
            output += "sign => [" + sign + "],\n\t";
            output += "treno => " + train +"\n);\n\n";
        }
    }
    return output;
}

//USED TO FIND THE MARKERBOARD ON THE CHUNK
string writerUMC::findMb(Route route,NetworkLayout nl,int linearId,const map<int,string> sC){
    for(int i = 0; i < (int)route.getSignals().size();i++){
        if(route.getSignals().at(i) == true){
            if(nl.getSignals().at(i).getSectionId() == linearId && nl.getSignals().at(i).getLinearEnd() == route.getDirection())
                return sC.find(nl.getSignals().at(i).getMbId())->second;
        }
    }
    return "null";
}

string writerUMC::pointObjectUmcOneRoute(Route route,const map<int,string> plC,NetworkLayout nl){
    string output;
    for(int i = 0; i < (int)route.getPath().size();i++){
        int current = route.getPath().at(i);
        if(current < (int)route.getPoints().size()){
            if(route.getPoints().at(current) != "INTER"){
                string name = plC.find(current)->second;
                output += (isdigit(name[0]) ? ("_"+name) :name) + ": Scambio (\n\t";
                output += "prev => [" + plC.find(route.getPath().at(i-1))->second +"],";
                output += "\n\t";
                output += "next => [" + plC.find(route.getPath().at(i+1))->second +"],";
                output += "\n\t";
                string conf = (route.getPoints().at(current) == "PLUS") ? "true" : "false";
                output += "conf => ["+ conf +"],\n\t";
                output += "treno => null\n);\n\n";
            }
        }
    }
    // Should I comment/decomment this??
    //Chain effect probably..
    if(output.empty()){
        //string prev,next;
        for(int i =0; i < (int)route.getPoints().size();i++ ){
            if(route.getPoints().at(i) != "INTER"){
                // if(nl.getPoints().at(i).getPlus() == route.getPath().back()) {
                //     prev = plC.find(nl.getPoints().at(i).getPlus())->second;
                //     next = plC.find(nl.getPoints().at(i).getStem())->second;
                // }
                // else if(nl.getPoints().at(i).getMinus() == route.getPath().back()){
                //     prev = plC.find(nl.getPoints().at(i).getMinus())->second;
                //     next = plC.find(nl.getPoints().at(i).getStem())->second;
                // }
                // else if(nl.getPoints().at(i).getStem() == route.getPath().back()){
                //     prev = plC.find(nl.getPoints().at(i).getStem())->second;
                //     next = route.getPoints().at(i) == "PLUS" ? plC.find(nl.getPoints().at(i).getPlus())->second :plC.find(nl.getPoints().at(i).getMinus())->second;
                // }
                // else if(prev == plC.find(nl.getPoints().at(i).getMinus())->second){
                //     prev = plC.find(nl.getPoints().at(i).getMinus())->second;
                //     next = plC.find(nl.getPoints().at(i).getStem())->second;
                // }
                // ----------------------
                //FIXME: find a solution to create all Scambio objects required
                // Could be inserted manually using only the config
                // cut off the "if" to visualize all points ( even blank);
                // ----------------------
                // if(!prev.empty() ){
                    string name = plC.find(nl.getPoints().at(i).sectionId)->second;
                    output += isdigit(name[0]) ? "_"+name : name +": Scambio (\n\t";
                    // prev and next should be "null"
                    // output += "prev => [" + prev + "],\n\t";
                    // output += "next => [" + next  +"],\n\t";
                    output += "prev => [null],\n\t";
                    output += "next => [null],\n\t";
                    string conf = route.getPoints().at(i) == "PLUS" ? "true" : "false";
                    output += "conf => ["+ conf +"],\n\t";
                    output += "treno => null\n);\n\n";
                // }
            } 
        }
    }
    return output;
}

string writerUMC::signalObjectUmcOneRoute(Route route,const map<int,string> plC,const map<int,string> sC,NetworkLayout nl){
    string output;
    int index;
    for(int i = 0; i < (int)route.getPath().size(); i++){
        if(route.getPath().at(i) >= (int)route.getPoints().size()){                                     
            index = route.getPath().at(i)-nl.getPoints().size(); 
            int linId = nl.getLinears().at(index).sectionId;
            string sign = findMb(route,nl,linId,sC);
            output += sign + " : Segnale(\n\t";
            output += "cdb => " + plC.find(route.getPath().at(i))->second + "\n);\n\n";
        }
    }
    return output;
}

string writerUMC::trainObjectUmcOneRoute(Route route,int number,const map<int,string> plC){
    string output;
    output += "train"+to_string(number)+" : Treno (\n\t";
    output += "id_itinerario => 0,\n\t";
    output += "nodi_itinerario => [";
    for(int i = 0; i < (int)route.getPath().size();i++){
        output += plC.find(route.getPath().at(i))->second +", ";
    }
    output = output.substr(0, output.length() - 2);
    output += "],\n\t";
    output += "nodo => " + plC.find(route.getPath().at(0))->second + "\n";
    output += ");\n";
    return output;
}

string writerUMC::derailAbsOneRoute(Route route,int number,const map<int,string> plC, NetworkLayout nl){
    string output;
    bool write= false;
    for(int i = 0; i < (int)route.getPath().size();i++){
        int current = route.getPath().at(i);
        if( current < (int)route.getPoints().size()){
            string point = plC.find(current)->second;
            output += "State : " + point + ".conf[0] == ";
            output += (route.getPoints().at(current) == "PLUS") ? "false" : "true";
            output += " and " + point + ".treno == train"+to_string(number);
            output += " and inState(train"+to_string(number)+".MOVIMENTO) -> DERAGLIAMENTO_train"+to_string(number)+"\n\t";
            write = true;
        }
    }
    if(write == false){
        for(int i = 0; i < (int)route.getPoints().size();i++){
            if(route.getPoints().at(i) != "INTER"){
                string point = plC.find(nl.getPoints().at(i).getSectionId())->second;
                output += "State : " + point + ".conf[0] == ";
                output += route.getPoints().at(i) == "PLUS" ? "false" : "true";
                output += " -> POSSIBILE_DERAGLIAMENTO\n\t";
            } 
        } 
    }
    return output;
}

string writerUMC::brokenSignalsOneRoute(Route route,const int number,const map<int,string> plC,const map<int,string> sC,NetworkLayout nl){
    string output;
    for(int i = 0; i < (int)route.getPath().size();i++){
        if(route.getPath().at(i) >= (int)route.getPoints().size()){                                     
            int index = route.getPath().at(i)-nl.getPoints().size(); 
            output += "State : inState(train"+to_string(number)+".MOVIMENTO) and ";
            int linId = nl.getLinears().at(index).sectionId;
            string sign = findMb(route,nl,linId,sC);
            output += sign + ".red == true";
            string init = plC.find(route.getPath().at(i))->second; 
            output += " and " + init + ".treno == train" + to_string(number) + " -> GUASTO_" + sign;
            output += "\n\t";
        }
    }
    return output;
}

//FIXME: Should I have to put all Points ( also if they are not in the path )??
// in UMC if I put elements, they must be declared as objects.
string writerUMC::abstractionUmcOneRoute(Route route,int number,const map<int,string> plC,const map<int,string> sC,const NetworkLayout nl){
    string output = "Abstractions{\n\t";
    output += "Action $1($*) -> $1($*)\n\t";
    output += "State : train"+to_string(number)+".nodo == " + plC.find(route.getPath().back())->second + " -> train"+to_string(number)+"_arrivato\n\t";
    output += derailAbsOneRoute(route,number,plC, nl);
    output += brokenSignalsOneRoute(route,number,plC,sC,nl);
    output += "\n}";
    return output;
}
string writerUMC::abstractionUmcTwoRoute(Route route1,Route route2,int n1, int n2,const map<int,string> plC,const map<int,string> sC,const NetworkLayout nl){
    string output = "Abstractions{\n\t";
    output += "Action $1($*) -> $1($*)\n\t";
    output += trainArrived          (route1,n1,plC);
    output += trainArrived          (route2,n2,plC);
    output += derailAbsOneRoute     (route1,n1,plC, nl);
    output += derailAbsOneRoute     (route2,n2,plC, nl);
    output += brokenSignalsOneRoute (route1,n1,plC,sC,nl);
    output += brokenSignalsOneRoute (route2,n2,plC,sC,nl);
    output += "\n}";
    return output;
}
string writerUMC::trainArrived(Route route,int number,const map<int,string> plC){
    string output;
    output += "State : train"+to_string(number)+".nodo == " + plC.find(route.getPath().back())->second + " -> train"+to_string(number)+"_arrivato\n\t";;
    return output; 
    // "State : train.nodo == " + plC.find(route.getPath().back())->second + " -> train_arrivato\n\t";
}
string writerUMC::stringCombinerNl(ParserXML *pXML, int i, int j){
    string s;
    s += pXML->getNl().toStringAdaptive(pXML->getIl().getRoutes().at(i),pXML->getPlCorrispondence(),pXML->getMbCorrispondence());
    s+= "\n\n";
    s += pXML->getNl().toStringAdaptive(pXML->getIl().getRoutes().at(j),pXML->getPlCorrispondence(),pXML->getMbCorrispondence());
    return s;
}

string writerUMC::stringCombinerIl(ParserXML *pXML, int i, int j){
    string s;
    s += pXML->getIl().getRoutes().at(i).toString(pXML->getIl().getMaxPathLength(), pXML->getIl().getMaxChunk());
    s += "\n\n"; 
    s += pXML->getIl().getRoutes().at(j).toString(pXML->getIl().getMaxPathLength(), pXML->getIl().getMaxChunk());
    return s;
}

string writerUMC::stringCombinerId(ParserXML *pXML, int i, int j){
    string s;
    s += to_string(pXML->getIl().getRoutes().at(i).getRouteId()) + "-";
    s += to_string(pXML->getIl().getRoutes().at(j).getRouteId());
    return s;
}

string writerUMC::pointObjectUmcTwoRoute(Route &route1, Route &route2,const map<int,string> plC, NetworkLayout nl){
    string output;
    vector<int> p1 = route1.getPath();
    vector<int> p2 = route2.getPath();
    bool find1, find2;
    int index1, index2;
    for(int i = 0;i < (int)nl.getPoints().size(); i++){
        if(route1.getPoints().at(i) != "INTER" ||route2.getPoints().at(i) != "INTER"){
            // auto it1 = find(p1.begin(),p1.end(), i);
            // auto it2 = find(p2.begin(),p2.end(), i);
            for(int j = 0; j < route1.getPath().size();j++){
                if(route1.getPath().at(j) == i){
                    find1 = true;
                    index1 = j;
                }      
            }
            for(int w = 0; w < route2.getPath().size();w++){
                if(route2.getPath().at(w) == i){
                    find2 = true;
                    index2 = w;
                }   
            }
            string name = plC.find(i)->second;
            output += (isdigit(name[0]) ? ("_"+name) :name) + ": Scambio (\n\t";
            string conf1 = (route1.getPoints().at(i) == "PLUS") ? "true" : "false";
            string conf2 = (route2.getPoints().at(i) == "PLUS") ? "true" : "false";
            if((find1 != true) && (find2 != true)){
                output += "prev => [null,null],";
                output += "\n\t";
                output += "next => [null,null]";
                output += "\n\t";
                output += "conf => ["+ conf1 +","+conf2+"],\n\t";
                output += "treno => null\n);\n\n";
            }else if((find1 == true) && (find2 != true)){
                string prova = plC.find(p1.at(index1-1))->second;
                output += "prev => [" + prova + ",null],",
                output += "\n\t";                
                output += "next => [" + plC.find(p1.at(index1+1))->second + ",null],",
                output += "\n\t";
                output += "conf => ["+ conf1 +","+conf2+"],\n\t";
                output += "treno => null\n);\n\n";
            }else if((find1 != true) && (find2 == true)){
                string prova = plC.find(p2.at(index2-1))->second;
                output += "prev => [null," + prova + "],",
                output += "\n\t";                
                output += "next => [null," + plC.find(p2.at(index2+1))->second + "],",
                output += "\n\t";
                output += "conf => ["+ conf1 +","+conf2+"],\n\t";
                output += "treno => null\n);\n\n";
            }else{
                output += "prev => [" + plC.find(p1.at(index1-1))->second + "," + plC.find(p2.at(index2-1))->second + "],",
                output += "\n\t";                
                output += "next => [" + plC.find(p1.at(index1+1))->second + "," + plC.find(p2.at(index2+1))->second + "],",
                output += "\n\t"; 
                output += "conf => ["+ conf1 +","+conf2+"],\n\t";
                output += "treno => null\n);\n\n";
            }   
        }
    }
    return output;
}

//     // Should I comment/decomment this??
//     //Chain effect probably..
//     if(output.empty()){
//         //string prev,next;
//         for(int i =0; i < (int)route1.getPoints().size();i++ ){
//             if(route1.getPoints().at(i) != "INTER"){
//                 // if(nl.getPoints().at(i).getPlus() == route.getPath().back()) {
//                 //     prev = plC.find(nl.getPoints().at(i).getPlus())->second;
//                 //     next = plC.find(nl.getPoints().at(i).getStem())->second;
//                 // }
//                 // else if(nl.getPoints().at(i).getMinus() == route.getPath().back()){
//                 //     prev = plC.find(nl.getPoints().at(i).getMinus())->second;
//                 //     next = plC.find(nl.getPoints().at(i).getStem())->second;
//                 // }
//                 // else if(nl.getPoints().at(i).getStem() == route.getPath().back()){
//                 //     prev = plC.find(nl.getPoints().at(i).getStem())->second;
//                 //     next = route.getPoints().at(i) == "PLUS" ? plC.find(nl.getPoints().at(i).getPlus())->second :plC.find(nl.getPoints().at(i).getMinus())->second;
//                 // }
//                 // else if(prev == plC.find(nl.getPoints().at(i).getMinus())->second){
//                 //     prev = plC.find(nl.getPoints().at(i).getMinus())->second;
//                 //     next = plC.find(nl.getPoints().at(i).getStem())->second;
//                 // }
//                 // ----------------------
//                 //FIXME: find a solution to create all Scambio objects required
//                 // Could be inserted manually using only the config
//                 // cut off the "if" to visualize all points ( even blank);
//                 // ----------------------
//                 // if(!prev.empty() ){
//                     string name = plC.find(nl.getPoints().at(i).sectionId)->second;
//                     output += isdigit(name[0]) ? "_"+name : name +": Scambio (\n\t";
//                     // prev and next should be "null"
//                     // output += "prev => [" + prev + "],\n\t";
//                     // output += "next => [" + next  +"],\n\t";
//                     output += "prev => [null],\n\t";
//                     output += "next => [null],\n\t";
//                     string conf = route1.getPoints().at(i) == "PLUS" ? "true" : "false";
//                     output += "conf => ["+ conf +"],\n\t";
//                     output += "treno => null\n);\n\n";
//                 // }
//             } 
//         }
//     }
//     return output;
// }
string writerUMC::linearObjectUmcTwoRoute(Route route1, Route route2, int i, map<int,string> plC, map<int,string> sC, NetworkLayout nl){
    return "null";
}
string writerUMC::trainObjectUmcTwoRoute (Route route1, Route route2, int i, map<int,string> plC){
    return "null";
}



// Abstractions{ 
//     Action $1($*) -> $1($*)
// --    State: Ventu.nodo == GA4  and GA4.treno == Ventu -> Ventu_arrivato
// --    State: Beppe.nodo == GA4 -> Beppe_arrivato
// --    State: Beppe.nodo == GA3 and Ventu.nodo == GA4 -> Tutti_arrivati
// --    State : W1.conf[0] == false and inState(Ventu.MOVIMENTO) -> DERAGLIAMENTO_VENTU
//     State : W3.conf[0] == true  and inState(Ventu.MOVIMENTO) and inState(Beppe.ARRIVATO) -> DERAGLIAMENTO_VENTU
//     State : W2.conf[1] == false and inState(Beppe.MOVIMENTO) and inState(Ventu.ATTESA_OK) -> DERAGLIAMENTO_BEPPE
//     State : W3.conf[1] == false and inState(Beppe.MOVIMENTO) and inState(Ventu.ATTESA_OK) -> DERAGLIAMENTO_BEPPE
//     State : inState(Ventu.MOVIMENTO) and S10.red == true  and GA5.treno == Ventu -> GUASTO_V
//     State : inState(Beppe.MOVIMENTO) and N2.red == true  and GA3.treno == Beppe -> GUASTO_B
//     State : Ventu.nodo /= null  and Ventu.nodo = Beppe.nodo -> incidente
//     State : inState(Ventu.ARRIVATO) and inState(Beppe.ARRIVATO) -> TUTTI_ARRIVATI 
//     State : inState(Ventu.ARRIVATO) and GA3.treno == Ventu -> Ventu_ARRIVATO 
//     State : inState(Beppe.ARRIVATO) and GA4.treno == Beppe -> Beppe_ARRIVATO 
//     Action: lostevent($e,$*) -> discarded_message
//     Action: $obj:Runtime_Error -> Design_Error
// Class Treno is

//   Signals:
//     ok, no;
    
//   Vars:
//     id_itinerario: int;
//     nodi_itinerario: obj[];
//     posizione: int := 0;
//     nodo: obj;

//   State Top = PRONTO, ATTESA_OK, MOVIMENTO, ARRIVATO, STOP

//   Transitions:
//     PRONTO    -> ATTESA_OK { - /
//                     a:obj:=nodi_itinerario[posizione];a.req(self, id_itinerario);
//                 }

//     ATTESA_OK -> PRONTO    { no }

//     ATTESA_OK -> MOVIMENTO { ok }

//     MOVIMENTO -> MOVIMENTO { - [posizione < nodi_itinerario.length-1] /
//                                    pos: int := posizione;
//                                    posizione := -1;
//                                    nodo := null;
//                                    b:obj:=nodi_itinerario[pos];b.sensoreOff(self, id_itinerario);
//                                    c:obj:=nodi_itinerario[pos+1];c.sensoreOn(self, id_itinerario);
//                                    posizione := pos+1;
//                                    nodo := nodi_itinerario[posizione];
//     }
    
//     MOVIMENTO -> ARRIVATO  { - [posizione = nodi_itinerario.length-1] }

// end Treno

// Class CircuitoDiBinario is

//   Signals:
//     req(sender: obj, id_itinerario: int);
//     ack(sender: obj, id_itinerario: int);
//     nack(sender: obj, id_itinerario: int);
//     commit(sender: obj, id_itinerario: int);
//     agree(sender: obj, id_itinerario: int);
//     disagree(sender: obj, id_itinerario: int);
//     green(id_itinerario:int);
//     red(id_itinerario:int);

//   Operations:
//     sensoreOn(sender: obj, id_itinerario);
//     sensoreOff(sender: obj, id_itinerario);

//   Vars:
//     next: obj[];
//     prev: obj[];
//     sign : obj[];
//     treno: obj := null;

//   State Top = NON_RISERVATO, ATTESA_ACK, ATTESA_COMMIT, ATTESA_AGREE, CHECK_LIGHT_START, RISERVATO, TRENO_IN_TRANSITO, CHECK_LIGHT_END
//   State CHECK_LIGHT_END Defers  req( sender: obj, id_itinerario: int), red(id_itinerario: int) ,nack(sender : obj, id_itinerario : int)

//   Transitions

//     NON_RISERVATO -> ATTESA_ACK { req(sender, id_itinerario)
//                        [(treno=null or sender=treno) and next[id_itinerario] /= null]   /
//                          d:obj:=next[id_itinerario];d.req(self, id_itinerario);}

//     NON_RISERVATO -> ATTESA_COMMIT { req(sender, id_itinerario)
//                        [(treno=null or sender=treno) and next[id_itinerario] = null] /
//                        e:obj:=prev[id_itinerario];e.ack(self, id_itinerario); }

//     ATTESA_ACK    -> ATTESA_COMMIT { ack(sender, id_itinerario) [prev[id_itinerario] /= null] /
//                        f:obj:=prev[id_itinerario];f.ack(self, id_itinerario); }

//     ATTESA_ACK    -> ATTESA_AGREE  { ack(sender, id_itinerario) [prev[id_itinerario] = null] /
//                        g:obj:=next[id_itinerario];g.commit(self, id_itinerario); }
                                  
//     ATTESA_COMMIT -> ATTESA_AGREE  { commit(sender, id_itinerario) [next[id_itinerario] /= null] /
//                                        h:obj:=next[id_itinerario];h.commit(self, id_itinerario);}

//     ATTESA_COMMIT -> Top.RISERVATO     { commit(sender, id_itinerario) [next[id_itinerario] = null and sign[id_itinerario] = null] /
//                         i:obj:=prev[id_itinerario];i.agree(self, id_itinerario);}
    
//     ATTESA_COMMIT -> CHECK_LIGHT_START {  commit(sender, id_itinerario) [ next[id_itinerario] = null and sign[id_itinerario] /= null ] /   
//                             a:obj:= sign[id_itinerario]; a.checkgreen(id_itinerario) }

//     ATTESA_AGREE  -> CHECK_LIGHT_START    { agree(sender, id_itinerario) [prev[id_itinerario] /= null and sign[id_itinerario] /= null] /
//                         a:obj:= sign[id_itinerario]; a.checkgreen(id_itinerario);}
                       
//     ATTESA_AGREE  -> Top.RISERVATO   { agree(sender, id_itinerario) [prev[id_itinerario] /= null and sign[id_itinerario] = null] /
//                     l:obj:=prev[id_itinerario];l.agree(self, id_itinerario); }
                    
//     ATTESA_AGREE  -> CHECK_LIGHT_START {agree(sender, id_itinerario) [prev[id_itinerario] = null and sign[id_itinerario] /= null] /     
//                                 a:obj:= sign[id_itinerario]; a.checkgreen(id_itinerario);
//                                 }
    
//     ATTESA_AGREE  -> Top.RISERVATO     { agree(sender, id_itinerario) [prev[id_itinerario] = null and sign[id_itinerario] = null] / 
//                          treno.ok;}
     
//     CHECK_LIGHT_START  -> Top.RISERVATO { green(id_itinerario)  [ prev[id_itinerario] = null and next[id_itinerario] /= null and sign[id_itinerario] /= null] /
//                             treno.ok
//                             }
                            
//     CHECK_LIGHT_START  -> Top.RISERVATO { green(id_itinerario)  [ next[id_itinerario] = null and prev[id_itinerario] /= null and sign[id_itinerario] /= null] /
//                             i:obj:=prev[id_itinerario];i.agree(self, id_itinerario);
//                             }
                            
//     CHECK_LIGHT_START  -> Top.RISERVATO { green(id_itinerario)  [ prev[id_itinerario] /= null and next[id_itinerario] /= null and sign[id_itinerario] /= null] /
//                             i:obj:=prev[id_itinerario];i.agree(self, id_itinerario);
//                             }                       
                            
//     CHECK_LIGHT_START  -> NON_RISERVATO { red(id_itinerario) [sign[id_itinerario] /= null and treno /= null ] /
//                             treno.no }

//     Top.RISERVATO     -> NON_RISERVATO { sensoreOff(sender, id_itinerario) [sign[id_itinerario] = null] /                        
//                         treno := null;}
 
//     Top.RISERVATO     -> CHECK_LIGHT_END { sensoreOff(sender, id_itinerario) [sign[id_itinerario] /= null] /
//                          a:obj:=sign[id_itinerario];a.checkred(id_itinerario);
//                          treno := null;}
    
//     Top.RISERVATO     -> TRENO_IN_TRANSITO { sensoreOn(sender, id_itinerario) / treno := sender; }

//     TRENO_IN_TRANSITO -> NON_RISERVATO { sensoreOff(sender, id_itinerario) [sign[id_itinerario] = null] / 
//                         treno := null;}

//     TRENO_IN_TRANSITO -> CHECK_LIGHT_END { sensoreOff(sender,id_itinerario) [sign[id_itinerario] /= null] /
//                         treno := null;
//                         a:obj:=sign[id_itinerario];a.checkred(id_itinerario) }
                        
//     CHECK_LIGHT_END   -> NON_RISERVATO { red(id_itinerario) [treno = null and next[id_itinerario] /= null]}
  
//     ATTESA_ACK    -> NON_RISERVATO { nack(sender, id_itinerario) /
//                        if  prev[id_itinerario] /= null  then
//                          { m:obj:=prev[id_itinerario];m.nack(self, id_itinerario) }
//                        else
//                          { treno.no }; }

//     ATTESA_COMMIT -> NON_RISERVATO { disagree(sender, id_itinerario) /
//                        if  next[id_itinerario] /= null  then
//                          { n:obj:=next[id_itinerario];n.disagree(self, id_itinerario) }; }

//     ATTESA_AGREE  -> NON_RISERVATO { disagree(sender, id_itinerario) [sign[id_itinerario] = null] /
//                        if  prev[id_itinerario] /= null  then
//                          { o:obj:=prev[id_itinerario];o.disagree(self, id_itinerario) }
//                        else
//                          { treno.no};}
                          
//     ATTESA_AGREE  -> CHECK_LIGHT_START { disagree(sender, id_itinerario) [sign[id_itinerario] /= null] /
//                        a:obj:=sign[id_itinerario]; a.checkred(id_itinerario)}
                         
//     CHECK_LIGHT_START    -> NON_RISERVATO { red(id_itinerario) [prev[id_itinerario] /= null] / 
//                         o:obj:=prev[id_itinerario];o.disagree(self, id_itinerario);}
                      
//     Top.RISERVATO     -> NON_RISERVATO { disagree(sender, id_itinerario)  [sign[id_itinerario] = null ] /
//                        if next[id_itinerario] /= null then
//                         { p:obj:=next[id_itinerario];p.disagree(self, id_itinerario) } 
//                        else
//                         { p:obj:=prev[id_itinerario];p.nack(self, id_itinerario) };}
            
//     Top.RISERVATO     -> NON_RISERVATO { nack(sender, id_itinerario) [prev[id_itinerario] /= null] / 
//                           p:obj:=prev[id_itinerario];p.nack(self,id_itinerario);
//                         }
//     Top.RISERVATO     -> CHECK_LIGHT_END { disagree(sender, id_itinerario) [sign[id_itinerario] /= null and next[id_itinerario] = null ]  /
//                        o:obj:=sign[id_itinerario];o.checkred(id_itinerario);}
                       
//     Top.RISERVATO     -> CHECK_LIGHT_END { disagree(sender, id_itinerario)  [sign[id_itinerario] /= null and next[id_itinerario] /= null ]  /
//                         p:obj:=next[id_itinerario];p.disagree(self, id_itinerario) ;
//                         o:obj:=sign[id_itinerario];o.checkred(id_itinerario);}
                        
//     CHECK_LIGHT_END -> NON_RISERVATO {  red(id_itinerario) [next[id_itinerario] = null] /
//                                     p:obj:=prev[id_itinerario];p.nack(self,id_itinerario);}
                                    
//     NON_RISERVATO -> NON_RISERVATO { nack(sender, id_itinerario) /
//                                     o:obj:=prev[id_itinerario];o.nack(self, id_itinerario);}
//     NON_RISERVATO -> NON_RISERVATO { req(sender, id_itinerario) [treno/=null and sender/=treno] /
//                        sender.nack(self, id_itinerario); }
                       
//     ATTESA_ACK        -> ATTESA_ACK        { req(sender, id_itinerario) / sender.nack(self, id_itinerario); }
//     ATTESA_COMMIT     -> ATTESA_COMMIT     { req(sender, id_itinerario) / sender.nack(self, id_itinerario); }
//     ATTESA_AGREE      -> ATTESA_AGREE      { req(sender, id_itinerario) / sender.nack(self, id_itinerario); }
//     Top.RISERVATO     -> Top.RISERVATO     { req(sender, id_itinerario) / sender.nack(self, id_itinerario); }
//     TRENO_IN_TRANSITO -> TRENO_IN_TRANSITO { req(sender, id_itinerario) / sender.nack(self, id_itinerario); }

// end CircuitoDiBinario

// class Segnale is 
//     Signals:
//         checkgreen(id_itinerario);
//         checkred(id_itinerario);
      
//     Vars:
//         cdb :obj;
//         red : bool:= true;
        
//     State Top = ROSSO , VERDE
//     State VERDE Defers checkred(id_itinerario)
//     State ROSSO Defers checkred(id_itinerario)

//     Transitions:
//      ROSSO -> VERDE { checkgreen(id_itinerario) [red = true] /
//                        cdb.green(id_itinerario);
//                         red = false; }
//      ROSSO -> ROSSO { checkred(id_itinerario) [red = true] /
//                         cdb.red(id_itinerario);
//                         red = true}  
                        
//      VERDE -> ROSSO { checkred(id_itinerario)  [red = false] /
//                         cdb.red(id_itinerario);
//                         red = true;}
                        
//      VERDE -> VERDE { checkgreen(id_itinerario) [red = false] /
//                         cdb.green(id_itinerario);
//                         }
// end Segnale

// Class Scambio is

//   Signals:
//     req(sender: obj, id_itinerario: int);
//     ack(sender: obj, id_itinerario: int);
//     nack(sender: obj, id_itinerario: int);
//     commit(sender: obj, id_itinerario: int);
//     agree(sender: obj, id_itinerario: int);
//     disagree(sender: obj, id_itinerario: int);

//   Operations:
//     sensoreOn(sender: obj, id_itinerario);
//     sensoreOff(sender: obj, id_itinerario);

//   Vars:
//     next: obj[];
//     prev: obj[];
//     conf: bool[];
//     rovescio: bool := False;
//     treno: obj := null;
//     itinerario: int;

//   State Top = NON_RISERVATO, ATTESA_ACK, ATTESA_COMMIT, ATTESA_AGREE, POSIZIONAMENTO, RISERVATO,CHECK_NON_RISERVATO, TRENO_IN_TRANSITO
//   State ATTESA_ACK Defers req(sender: obj, id_itinerario: int)
//   State ATTESA_ACK Defers nack(sender,id_itinerario)

//  Transitions:

//     NON_RISERVATO  -> ATTESA_ACK     { req(sender, id_itinerario) /
//                    q:obj:=next[id_itinerario];q.req(self, id_itinerario); }

//     ATTESA_ACK     -> ATTESA_COMMIT  { ack(sender, id_itinerario) /
//                    r:obj:=prev[id_itinerario];r.ack(self, id_itinerario); }

//     ATTESA_COMMIT  -> ATTESA_AGREE   { commit(sender, id_itinerario) /
//                    s:obj:=next[id_itinerario];s.commit(self, id_itinerario); }

//     ATTESA_AGREE   -> Top.RISERVATO      { agree(sender, id_itinerario) [rovescio = conf[id_itinerario]]/
//                                        t:obj:=prev[id_itinerario];t.agree(self, id_itinerario); }

//     ATTESA_AGREE   -> POSIZIONAMENTO { agree(sender, id_itinerario) [rovescio /= conf[id_itinerario]] /
//                                         itinerario := id_itinerario;}

//     POSIZIONAMENTO -> Top.RISERVATO      { - /
//                                            rovescio := not rovescio;
//                        u:obj:=prev[itinerario];u.agree(self, itinerario); }
                       
//     POSIZIONAMENTO -> CHECK_NON_RISERVATO { - /
//                        z:obj:=next[itinerario];z.disagree(self, itinerario);}
                       
//     CHECK_NON_RISERVATO -> NON_RISERVATO { nack(sender,id_itinerario) /
//                         v:obj:=prev[id_itinerario];v.disagree(self, id_itinerario);}
      
//     NON_RISERVATO    -> NON_RISERVATO { nack(sender,id_itinerario)/
//                         v:obj:=prev[id_itinerario];v.nack(self,id_itinerario);}
                        
//     Top.RISERVATO     -> TRENO_IN_TRANSITO { sensoreOn(sender, id_itinerario) / treno := sender; }

//     TRENO_IN_TRANSITO -> NON_RISERVATO { sensoreOff(sender, id_itinerario) / 
//                                         treno := null;}

//     ATTESA_ACK     -> NON_RISERVATO { nack(sender, id_itinerario) /
//                         a:obj:=prev[id_itinerario];a.nack(self, id_itinerario); }

//     ATTESA_COMMIT  -> NON_RISERVATO { disagree(sender, id_itinerario) /
//                         b:obj:=next[id_itinerario];b.disagree(self, id_itinerario); }

//     ATTESA_AGREE   -> NON_RISERVATO { disagree(sender, id_itinerario) /
//                         c:obj:=prev[id_itinerario];c.disagree(self, id_itinerario); }

//     POSIZIONAMENTO -> NON_RISERVATO { disagree(sender, id_itinerario) /
//                         d:obj:=next[id_itinerario];d.disagree(self, id_itinerario); }

//     Top.RISERVATO      -> NON_RISERVATO { disagree(sender, id_itinerario) /
//                         e:obj:=next[id_itinerario];e.disagree(self, id_itinerario); }

//     ATTESA_COMMIT     -> ATTESA_COMMIT     { req(sender, id_itinerario) / sender.nack(self, id_itinerario); }
//     ATTESA_AGREE      -> ATTESA_AGREE      { req(sender, id_itinerario) / sender.nack(self, id_itinerario); }
//     POSIZIONAMENTO    -> POSIZIONAMENTO    { req(sender, id_itinerario) / sender.nack(self, id_itinerario); }
//     Top.RISERVATO     -> Top.RISERVATO     { req(sender, id_itinerario) / sender.nack(self, id_itinerario); }
//     TRENO_IN_TRANSITO -> TRENO_IN_TRANSITO { req(sender, id_itinerario) / sender.nack(self, id_itinerario); }

// end Scambio