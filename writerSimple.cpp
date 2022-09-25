#include "writerSimple.hpp"
#include <string>

void writerSimple::writeFile(const string outputFile,ParserXML *pXML,int train, int select, int route1, int route2){
    // 1. create a file.txt for each route and add an extra routes that continue
    // 2. create a file.txt that contents all routes
    // 3. create a file.txt that contents some routes
    if(outputFile != ""){ 
        string outputFiletxt = outputFile + "NetworkLayout-Interlocking.txt";      
        try{
            ofstream myfile;
            myfile.open(outputFiletxt);
            myfile << "\n/* NetworkLayout */\n\n";
            myfile << pXML->getNl().toString(pXML->getPlCorrispondence(),pXML->getMbCorrispondence());
            myfile << "\n\n/* NetworkLayout End */\n\n";
            myfile << "\n/* Interlocking */\n\n";
            myfile << pXML->getIl().toString() + "\n";
            myfile << "\n/* Interlocking End */\n";
            myfile.close();
            cout << "Successfully wrote to the file."<<endl;
        }catch(const exception& e){
            cerr << "An error occured" << endl;
        }
    }
}