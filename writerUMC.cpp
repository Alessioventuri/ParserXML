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
                myfile << defaultUMCsetup(nl,il,i,pl);
                myfile.close();
                cout << "Successfully wrote to the file."<<endl;
            }catch(const exception& e){
                cerr << "An error occured" << endl;
            }
        }
    } 
}

string writerUMC::defaultUMCsetup(NetworkLayout nl,Interlock il,int i,map<int,string> pl){
    string s;
    
    s += "Objects:\n";
   // s += pointObjectUMC(il.getRoutes().at(i),);
    s += "\n\n";
    s += linearObjectUMC(il.getRoutes().at(i),pl,nl);
    s += "\n\n";
   // s += signalObjectUMC();
    return s;
}
string writerUMC::linearObjectUMC(Route route,map<int,string> pl,NetworkLayout nl){
    string output;
    int index;
    for(int i = 0; i < route.getPath().size();i++){
        if(route.getPath().at(i) >= route.getPoints().size()){                                     
            index = route.getPath().at(i)-nl.getPoints().size(); 
            string up  = nl.getLinears().at(index).getUpNeig() != -1 ? pl.find(nl.getLinears().at(index).getUpNeig())->second : "null";
            string down = nl.getLinears().at(index).getDownNeig() != -1 ? pl.find(nl.getLinears().at(index).getDownNeig())->second : "null";
            if(route.getDirection() == "up"){
                output += pl.find(nl.getLinears().at(index).sectionId)->second + ": CircuitoDiBinario (\n\t";
                output += "prev => [" + down + "],";
                output += "\n\t";
                output += "next => [" + up + "],";
                //TODO: do the train if it is the initial linear
                output += "\n);\n";
            }
            else{
                output += pl.find(nl.getLinears().at(index).sectionId)->second + ": CircuitoDiBinario (\n\t";
                output += "prev => [" + up + "],";
                output += "\n\t";
                output += "next => [" + down + "],";
                //TODO: do signals and train if it is the initial linear
                output += "\n);\n";
            }
        }
    }
    return output;
}

// string NetworkLayout::linearStringAdaptive(Route route,map<int,string> plC){
//     string output;
//     for(int i = 0; i < route.getPath().size();i++){
//         //path contains index of linears and linears start when points end 
//         //(ie. path{8},points has index (0,1,2,3,4), and linears start from 5, so path.at(i)-points.size() ( 8-5 = 3) 
//         if(route.getPath().at(i) >= points.size())
//             output += linears.at(route.getPath().at(i)-points.size()).toString(plC) +",\n";
//     }
//     if (output.length() > 0)
//         output = output.substr(0, output.length() - 2);
//     return output;
// }
// string Linear::toString(map<int,string> pl){
// 	string secId = pl.find(sectionId)->second;
// 	string upN =  upNeighbour != -1 ? pl.find(upNeighbour)->second : "null";
// 	string downN = downNeighbour != -1 ? pl.find(downNeighbour)->second : "null";
// 	return  "linearUp[" + secId + " - "+ to_string(sectionId)+ "] = " + upN + " - " + to_string(upNeighbour)
// 			 + ",\nlinearDown[" + secId + " - "+ to_string(sectionId)+ "] = " + downN + " - " + to_string(downNeighbour);

// }

// string writerUMC::pointObjectUMC(Route route, map<int,string> pl,NetworkLayout nl){
//     string output;
//     for(int i = 0; i < route.getPoints().size();i++){
//         if(route.getDirection() == "up"){
//             if(route.getPoints.at(i) != "INTER"){
//                 output += pl.find(nl.getPoints().at(i).sectionId)->second +": Scambio (\n";
//                 output += "prev => [" +pl.find(nl.getPoints().at(i).getStem())->second +"],\n";
//                 if(route.getPoints().at(i) == "PLUS"){
//                     output += "next => [" +pl.find(nl.getPoints().at(i).getPlus())->second +"],\n";
//                     output += "conf => [true],\n";
//                     output += "treno => null";
//                 }else{
//                     output += "next => [" +pl.find(nl.getPoints().at(i).getMinus())->second +"],\n";
//                     output += "conf => [false],\n";
//                     output += "treno => null";
//                 }
//             }
//         }else{
//             if(route.getPoints.at(i) != "INTER"){
//                 output += pl.find(nl.getPoints().at(i).sectionId)->second +": Scambio (\n";
//                 output += "prev => [" +pl.find(nl.getPoints().at(i).getStem())->second +"],\n";
//                 if(route.getPoints().at(i) == "PLUS"){
//                     output += "next => [" +pl.find(nl.getPoints().at(i).getPlus())->second +"],\n";
//                     output += "conf => [true],\n";
//                     output += "treno => null";
//                 }else{
//                     output += "next => [" +pl.find(nl.getPoints().at(i).getMinus())->second +"],\n";
//                     output += "conf => [false],\n";
//                     output += "treno => null";
//                 }
//             }            
//         }
//     }
    
// }

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