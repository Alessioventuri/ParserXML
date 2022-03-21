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
    
    s += "Objects:\n";
   // s += pointObjectUMC(il.getRoutes().at(i),);
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
            if(route.getDirection() == "up"){
                output += pl.find(nl.getLinears().at(index).sectionId)->second + ": CircuitoDiBinario (\n\t";
                output += "prev => [" + down + "],";
                output += "\n\t";
                output += "next => [" + up + "],";
                output += "\n\t";
                output += "sign => [" + sign + "],";
                output += "\n\t";
                output += "treno => " + train;
                output += "\n);\n\n";
            }else{
                output += pl.find(nl.getLinears().at(index).sectionId)->second + ": CircuitoDiBinario (\n\t";
                output += "prev => [" + up + "],";
                output += "\n\t";
                output += "next => [" + down + "],";
                output += "\n\t";
                output += "sign => [" + sign + "],";
                output += "\n\t";
                output += "treno => " + train;
                output += "\n);\n\n";
            }
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

// string Signal::toString(map<int,string> sC,map<int,string> plC){
//     string mId = sC.find(mbId)->second;
//     //cout << mId << " " <<  to_string(sectionId) << " "<< linearEnd<< endl;
//     return "mbSection[" + mId + " - " + to_string(mbId) + "] = " + plC.find(sectionId)->second + " - " + to_string(sectionId)
//             + ",\nmbLinearEnd[" + mId + " - " + to_string(mbId) + "] = " + linearEnd;
// }
// string NetworkLayout::signalStringAdaptive(Route route,map<int,string> sC,map<int,string> plC){
//     string output;
//     for(int i = 0; i< route.getSignals().size();i++){
//         if(route.getSignals().at(i) == true)
//             output += signals.at(i).toString(sC,plC) + ",\n";
//     }
//     if (output.length() > 0)
//         output = output.substr(0, output.length() - 2);
//     return output;
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