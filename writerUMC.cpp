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
    s += linearObjectUMC(il.getRoutes().at(i),pl,sC,nl);
    s += signalObjectUMC(il.getRoutes().at(i),pl,sC,nl);
    s += trainObjectUMC(il.getRoutes().at(i),pl,nl);
    s += abstractionUMC(il.getRoutes().at(i),pl,sC,nl);
    return s;
}

string writerUMC::linearObjectUMC(Route route,map<int,string> plC,map<int,string> sC, NetworkLayout nl){
    string output;
    int index;
    for(int i = 0; i < route.getPath().size();i++){
        if(route.getPath().at(i) >= route.getPoints().size()){                                     
            index = route.getPath().at(i)-nl.getPoints().size(); 
            string up  = nl.getLinears().at(index).getUpNeig() != -1 ? plC.find(nl.getLinears().at(index).getUpNeig())->second : "null";
            string down = nl.getLinears().at(index).getDownNeig() != -1 ? plC.find(nl.getLinears().at(index).getDownNeig())->second : "null";
            string sign = findMb(route,nl,nl.getLinears().at(index).sectionId,sC);
            string train = (nl.getLinears().at(index).sectionId == route.getPath().at(0)) ? "train" : "null";
            output += plC.find(nl.getLinears().at(index).sectionId)->second + ": CircuitoDiBinario (\n\t";
            output += "prev => [" + (route.getDirection() == "up" ? down : up) + "],\n\t";
            output += "next => [" + (route.getDirection() == "up" ? up : down) + "],";
            output += "sign => [" + sign + "],\n\t";
            output += "treno => " + train +"\n);\n\n";
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

string writerUMC::pointObjectUMC(Route route, map<int,string> plC,NetworkLayout nl){
    string output;
    for(int i = 0; i < route.getPath().size();i++){
        int current = route.getPath().at(i);
        if(current < route.getPoints().size()){
            if(route.getPoints().at(current) != "INTER"){
                output += plC.find(current)->second +": Scambio (\n\t";
                output += "prev => [" + plC.find(route.getPath().at(i-1))->second +"],";
                output += "\n\t";
                output += "next => [" + plC.find(route.getPath().at(i+1))->second +"],";
                output += "\n\t";
                string conf = route.getPoints().at(i) == "PLUS" ? "true" : "false";
                output += "conf => ["+ conf +"],\n\t";
                output += "treno => null\n);\n\n";
            }
        }
    }
    if(output.empty()){
        string prev,next;
        for(int i =0; i < route.getPoints().size();i++ ){
            if(route.getPoints().at(i) != "INTER"){
                if(nl.getPoints().at(i).getPlus() == route.getPath().back()) {
                    prev = plC.find(nl.getPoints().at(i).getPlus())->second;
                    next = plC.find(nl.getPoints().at(i).getStem())->second;
                }
                else if(nl.getPoints().at(i).getMinus() == route.getPath().back()){
                    prev = plC.find(nl.getPoints().at(i).getMinus())->second;
                    next = plC.find(nl.getPoints().at(i).getStem())->second;
                }
                else if(nl.getPoints().at(i).getStem() == route.getPath().back()){
                    prev = plC.find(nl.getPoints().at(i).getStem())->second;
                    next = route.getPoints().at(i) == "PLUS" ? plC.find(nl.getPoints().at(i).getPlus())->second :plC.find(nl.getPoints().at(i).getMinus())->second;
                }
                else if(prev == plC.find(nl.getPoints().at(i).getMinus())->second){
                    prev = plC.find(nl.getPoints().at(i).getMinus())->second;
                    next = plC.find(nl.getPoints().at(i).getStem())->second;
                }
                //TODO: find a solution to create all Scambio objects required
                // Could be inserted manually using only the config
                // cut off the "if" to visualize all points ( even blank);
              //  if(!prev.empty() ){
                    output += plC.find(nl.getPoints().at(i).sectionId)->second +": Scambio (\n\t";
                    output += "prev => [" + prev +"],\n\t";
                    output += "next => [" + next  +"],\n\t";
                    string conf = route.getPoints().at(i) == "PLUS" ? "true" : "false";
                    output += "conf => ["+ conf +"],\n\t";
                    output += "treno => null\n);\n\n";
              //  }
            } 
        }

    }
    return output;
}

string writerUMC::signalObjectUMC(Route route, map<int,string> plC, map<int,string> sC,NetworkLayout nl){
    string output;
    int index;
    for(int i = 0; i < route.getSignals().size();i++){
        if(route.getSignals().at(i) == true){   
            string sign = sC.find(nl.getSignals().at(i).getMbId())->second;                                  
            output += sign + " : Segnale(\n\t";
            output += "cdb => " + plC.find(nl.getSignals().at(i).getSectionId())->second + "\n);\n\n";
            }
    }
    return output;
}

string writerUMC::trainObjectUMC(Route route, map<int,string> plC, NetworkLayout nl){
    string output;
    output += "train : Treno (\n\t";
    output += "id_itinerario => 0,\n\t";
    output += "nodi_itinerario => [";
    for(int i = 0; i < route.getPath().size();i++){
        output += plC.find(route.getPath().at(i))->second +", ";
    }
    output = output.substr(0, output.length() - 2);
    output += "],\n\t";
    output += "nodo => " + plC.find(route.getPath().at(0))->second + "\n";
    output += ");\n";
    return output;
}


//TODO: Should I have to put all Points ( also if they are not in the path )??
string writerUMC::abstractionUMC(Route route,map<int,string> plC,map<int,string> sC,NetworkLayout nl){
    string output = "Abstractions{\n\t";
    bool write = false;
    output += "State : train.nodo == " + plC.find(route.getPath().back())->second + " -> train_arrivato\n\t";
    for(int i = 0; i < route.getPath().size();i++){
        int current = route.getPath().at(i);
        if( current < route.getPoints().size()){
            string point = plC.find(current)->second;
            output += "State : " + point + ".conf[0] == ";
            output += route.getPoints().at(i) == "PLUS" ? "false" : "true";
            output += " and inState(train.MOVIMENTO) -> DERAGLIAMENTO_train\n\t";
            write = true;
        }
    }
    if(write == false){
        for(int i = 0; i < route.getPoints().size();i++){
            if(route.getPoints().at(i) != "INTER"){
                string point = plC.find(nl.getPoints().at(i).getSectionId())->second;
                output += "State : " + point + ".conf[0] == ";
                output += route.getPoints().at(i) == "PLUS" ? "false" : "true";
                output += " -> DERAGLIAMENTO_train\n\t";
            } 
        } 
    }
    output += "\n}";
    return output;
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