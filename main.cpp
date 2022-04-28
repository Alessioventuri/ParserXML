#include "Interlock.hpp"
#include "NetworkLayout.hpp"
#include "writer.hpp"
#include "writerUMC.hpp"
#include "writerSimple.hpp"
#include "ParserXML.hpp"
#include <stdexcept>
#include <iostream>
#include <stdlib.h>
#include <fstream>

#include <unordered_map>
#include <string>
#include <cstring>
#include <exception>

#include <sys/stat.h>
#include <sys/types.h>
#include <sstream>

using namespace rapidxml;
using namespace std;

writer* writer::write(fileType type){
    if(type == fileType::UMCFile) 
        return new writerUMC();
    if(type == fileType::SimpleFile)
        return new writerSimple();
    return NULL;
}
int ParserXML::count = 0;

int main(int argc, char *argv[]){
    // bool outputToFile = false;
    string input;
    // string input = "/Users/alessioventuri/Desktop/XML/lvr_7_right_rt.xml";
    string outputFile;
    // string outputFile = "/Users/alessioventuri/Desktop/Interlocking/";
    bool helpCalled = false;
    
    for(int i = 0; i < argc; i++){
        string stringInput = argv[i];
        cout << stringInput << endl;
        if(stringInput == ("-i")){
            if(argc <= i+1 && !helpCalled) 
                throw invalid_argument("There must be an input file");
            input = argv[i+1];
        }
        if(stringInput == "-o"){
            if(argc >i+1)
                outputFile = argv[i+1];
        }
        if(stringInput == "-h"){
            helpCalled = true;
            break;
        }
    }
    if(input == "" && !helpCalled)
        throw invalid_argument("There must be an input file");
    if (helpCalled){
        cout << "-i <inputfile> ";
        cout << "Provides the input xml file that should be parsed" << endl;
        cout << "-o <outputfile> ";
        cout << "Provides the output text which the program should write the config data"<<endl;
        cout << "This remove the printout in the terminal and is optional"<< endl;
        cout << "-h ";
        cout << "Shows this info" << endl;

        exit(0);

    }

    ParserXML *pXML = new ParserXML(input);
    
    cout << outputFile << endl; 
    outputFile = outputFile + (pXML)->SplitFilename(input) + "/";
    cout << outputFile << endl;
    std::stringstream ss;
    ss <<  outputFile;
    int rc = mkdir(ss.str().c_str(), 0777);
    if(rc == 0) std::cout << "Created " << ss.str() << " success\n";
    int file;
    cout << "WHICH TYPE DO YOU WANT" <<endl;
    cout << "0 : UMCFile" << endl;
    cout << "1 : SimpleFile " << endl;
    cin >> file;
    // outputFile = outputFile + "SingleRoute/";
    // writer* obj = writer::write(UMCFile);
    // cout << outputFile << endl;
    // std::stringstream sss;
    // sss <<  outputFile;
    // int rc1 = mkdir(sss.str().c_str(), 0777);
    // if(rc1 == 0) std::cout << "Created " << sss.str() << " success\n";
    // //Interlock il_comb = il.routeCombiner(nl,il);
    // obj->writeFile(outputFile,pXML,1);
    // }
    if(file == 0){
        outputFile = outputFile  + "UMC/";
        cout << outputFile << endl;
        std::stringstream ss;
        ss <<  outputFile;
        int rc = mkdir(ss.str().c_str(), 0777);
        if(rc == 0) std::cout << "Created " << ss.str() << " success\n";

        writer* obj = writer::write(UMCFile);
        int choose;
        cout << "DO YOU WANT TO COMBINE ROUTE WITH LENGTH < 4 ?" <<endl;
        cout << "PRESS:" << endl;
        cout << "0 : Yes" << endl;
        cout << "1 : No"  <<endl;
        cin >> choose;
        if(choose == 0){
            outputFile = outputFile + "CombinedRoutes/";
            cout << outputFile << endl;
            std::stringstream ss;
            ss <<  outputFile;
            int rc = mkdir(ss.str().c_str(), 0777);
            if(rc == 0) std::cout << "Created " << ss.str() << " success\n";
            Interlock il_comb = pXML->getIl().routeCombiner(pXML->getNl(),pXML->getIl());
            pXML->setIl(il_comb);
            // cout << il_comb.toStringCombiner() << endl;
            obj->writeFile(outputFile,pXML,choose);
        }
        else{
            outputFile = outputFile + "singleRoute/";
            cout << outputFile << endl;
            std::stringstream ss;
            ss <<  outputFile;
            int rc = mkdir(ss.str().c_str(), 0777);
            if(rc == 0) std::cout << "Created " << ss.str() << " success\n";
            obj->writeFile(outputFile,pXML,choose);
        }
    
        delete obj,pXML;
    }
    if(file == 1){
        writer* obj = writer::write(SimpleFile);
        outputFile = outputFile  + "Simple/";
        cout << outputFile << endl;
        std::stringstream ss;
        ss <<  outputFile;
        int rc = mkdir(ss.str().c_str(), 0777);
        if(rc == 0) std::cout << "Created " << ss.str() << " success\n";
        obj->writeFile(outputFile,pXML,0);
        delete obj,pXML;
    }
}

