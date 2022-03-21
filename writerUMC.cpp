#include "writerUMC.hpp"
#include "helpFunction.hpp"
#include <string>

void writerUMC::writeFile(string outputFile,NetworkLayout nl,Interlock il,map<int,string> pl,map<int,string> mb){
    // 1. create a file.txt for each route and add an extra routes that continue
    // 2. create a file.txt that contents all routes
    // 3. create a file.txt that contents some routes
    il.generateMaxChunk();
    for ( int i = 0 ; i < il.getRoutes().size(); i++ ){
        if(outputFile != ""){ 
            string outputFiletxt = outputFile + "route" + to_string(il.getRoutes().at(i).getRouteId()) + ".txt";      
            try{
                ofstream myfile;
                myfile.open(outputFiletxt);
                // myfile << "/* Corrispondence Linears-Points */\n";
                // myfile << getCorrispondenceLinearPoint(il.getRoutes().at(i),nl,pl);
                // myfile << "\n/* Corrispondence Linears-Points End */\n\n";
                // myfile << "\n/* Corrispondence Signals */\n";
                // myfile << getCorrispondenceSignal(il.getRoutes().at(i),nl,mb);
                // myfile << "\n/* Corrispondence Signals End */\n\n";
                myfile << "\n/* NetworkLayout */\n\n";
                myfile << nl.toStringAdaptive(il.getRoutes().at(i),pl,mb);
                myfile << "\n\n/* NetworkLayout End */\n\n";
                myfile << "\n/* Interlocking */\n\n";
                myfile << il.getRoutes().at(i).toString(il.getMaxPathLenght(), il.getMaxChunk()) + "\n";
                myfile << "\n/* Interlocking End */\n";
                myfile << "\n/* UMC code */\n";
                myfile << defaultUMCsetup(nl,il,i,pl,mb);
                myfile.close();
                cout << "Successfully wrote to the file."<<endl;
            }catch(const exception& e){
                cerr << "An error occured" << endl;
            }
        }
    } 
}

string writerUMC::defaultUMCsetup(NetworkLayout nl,Interlock il,int i,map<int,string> pl,map<int,string> sC){
    string s;
    
    s += "Objects:\n\n";
    s += pointObjectUMC(il.getRoutes().at(i),pl,nl);
    s += "\n";
    s += linearObjectUMC(il.getRoutes().at(i),pl,sC,nl);
    s += "\n";
   // s += signalObjectUMC();
    return s;
}

string writerUMC::linearObjectUMC(Route route,map<int,string> pl,map<int,string> sC, NetworkLayout nl){
    string output;
    int index;
    for(int i = 0; i < route.getPath().size();i++){
        if(route.getPath().at(i) >= route.getPoints().size()){                                     
            index = route.getPath().at(i)-nl.getPoints().size(); 
            string up  = nl.getLinears().at(index).getUpNeig() != -1 ? pl.find(nl.getLinears().at(index).getUpNeig())->second : "null";
            string down = nl.getLinears().at(index).getDownNeig() != -1 ? pl.find(nl.getLinears().at(index).getDownNeig())->second : "null";
            string sign = findMb(route,nl,nl.getLinears().at(index).sectionId,sC);
            string train = (nl.getLinears().at(index).sectionId == route.getPath().at(0)) ? "train" : "null";
            output += pl.find(nl.getLinears().at(index).sectionId)->second + ": CircuitoDiBinario (\n\t";
            output += "prev => [" + (route.getDirection() == "up" ? down : up) + "],";
            output += "\n\t";
            output += "next => [" + (route.getDirection() == "up" ? up : down) + "],";
            output += "\n\t";
            output += "sign => [" + sign + "],";
            output += "\n\t";
            output += "treno => " + train;
            output += "\n);\n\n";
        }
    }
    return output;
}

//USED TO FIND THE MARKERBOARD ON THE CHUNK
string writerUMC::findMb(Route route,NetworkLayout nl,int linearId,map<int,string> sC){
    for(int i = 0; i < route.getSignals().size();i++){
        if(route.getSignals().at(i) == true){
            if(nl.getSignals().at(i).getSectionId() == linearId && nl.getSignals().at(i).getLinearEnd() == route.getDirection())
                return sC.find(nl.getSignals().at(i).getMbId())->second;
        }
    }
    return "null";
}


string writerUMC::pointObjectUMC(Route route, map<int,string> pl,NetworkLayout nl){
    string output;
    for(int i = 0; i < route.getPath().size();i++){
        int current = route.getPath().at(i);
        if(current < route.getPoints().size()){
            if(route.getPoints().at(current) != "INTER"){
                output += pl.find(current)->second +": Scambio (\n\t";
                output += "prev => [" + pl.find(route.getPath().at(i-1))->second +"],\n\t";
                output += "next => [" + pl.find(route.getPath().at(i+1))->second +"],\n\t";
                string conf = route.getPoints().at(i) == "PLUS" ? "true" : "false";
                output += "conf => ["+ conf +"],\n\t";
                output += "treno => null\n);\n\n";
            }
        }
    }
    if(output.empty()){
        string prev,next;
        for(int i = route.getPoints().size()-1; i > -1;i-- ){
            if(route.getPoints().at(i) != "INTER"){
                if(nl.getPoints().at(i).getPlus() == route.getPath().back()) {
                    prev = pl.find(nl.getPoints().at(i).getPlus())->second;
                    next = pl.find(nl.getPoints().at(i).getStem())->second;
                }
                else if(nl.getPoints().at(i).getMinus() == route.getPath().back()){
                    prev = pl.find(nl.getPoints().at(i).getMinus())->second;
                    next = pl.find(nl.getPoints().at(i).getStem())->second;
                }
                else if(nl.getPoints().at(i).getStem() == route.getPath().back()){
                    prev = pl.find(nl.getPoints().at(i).getStem())->second;
                    next = route.getPoints().at(i) == "PLUS" ? pl.find(nl.getPoints().at(i).getPlus())->second :pl.find(nl.getPoints().at(i).getMinus())->second;
                }
                //TODO: find a solution to create all Scambio objects required
                if(!prev.empty() ){
                    output += pl.find(nl.getPoints().at(i).sectionId)->second +": Scambio (\n\t";
                    output += "prev => [" + prev +"],\n\t";
                    output += "next => [" + next  +"],\n\t";
                    string conf = route.getPoints().at(i) == "PLUS" ? "true" : "false";
                    output += "conf => ["+ conf +"],\n\t";
                    output += "treno => null\n);\n\n";
                }
            } 
        }

    }
    return output;
        // if(route.getDirection() == "up"){
        //     if(route.getPoints().at(i) != "INTER"){
        //         output += pl.find(nl.getPoints().at(i).sectionId)->second +": Scambio (\n\t";
        //         if(route.getPath()\.getPoints().at(i).sectionId )
        //         output += "prev => [" +pl.find(nl.getPoints().at(i).getStem())->second +"],\n\t";
        //         if(route.getPoints().at(i) == "PLUS"){
        //             output += "next => [" +pl.find(nl.getPoints().at(i).getPlus())->second +"],\n\t";
        //             output += "conf => [true],\n\t";
        //             output += "treno => null";
        //             output += "\n);";
        //         }else{
        //             output += "next => [" +pl.find(nl.getPoints().at(i).getMinus())->second +"],\n";
        //             output += "conf => [false],\n\t";
        //             output += "treno => null";
        //             output += "\n);";
        //         }
        //     }
        // }else{
        //     if(route.getPoints().at(i) != "INTER"){
        //         output += pl.find(nl.getPoints().at(i).sectionId)->second +": Scambio (\n\t";
        //         output += "prev => [" +pl.find(nl.getPoints().at(i).getStem())->second +"],\n\t";
        //         if(route.getPoints().at(i) == "PLUS"){
        //             output += "next => [" +pl.find(nl.getPoints().at(i).getPlus())->second +"],\n\t";
        //             output += "conf => [true],";
        //             output += "\n\t";
        //             output += "treno => null";
        //             output += "\n);";
        //         }else{
        //             output += "next => [" +pl.find(nl.getPoints().at(i).getMinus())->second +"],";
        //             output += "\n\t";
        //             output += "conf => [false],";
        //             output += "\n\t";
        //             output += "treno => null";
        //             output += "\n);";
        //         }
        //     }            
        // }
    
    
}

// string NetworkLayout::pointStringAdaptive(Route route,map<int,string> pl){
//     string output; 
//     for(int i = 0; i < route.getPoints().size();i++){
//         if(route.getPoints().at(i) != "INTER")
//             output += points.at(i).toString(pl) +",\n";
//     }
//     if (output.length() > 0)
//         output = output.substr(0, output.length() - 2);
//     return output;
// }