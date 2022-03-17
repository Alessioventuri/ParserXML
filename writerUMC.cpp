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
                myfile.close();
                cout << "Successfully wrote to the file."<<endl;
            }catch(const exception& e){
                cerr << "An error occured" << endl;
            }
        }
    } 
}
//TODO: create object for UMC code
void writerUMC::defaultUMCsetup(){

}