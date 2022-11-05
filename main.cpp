#include "NetworkLayout.hpp"
#include "writerUMC.hpp"
#include "writerSimple.hpp"
#include "ParserXML.hpp"
#include <stdexcept>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <memory>

#include <unordered_map>
#include <string>
#include <cstring>
#include <exception>

#include <sys/stat.h>
#include <sys/types.h>
#include <sstream>
#include <memory>

using namespace rapidxml;
using namespace std;

unique_ptr<writer> writer::write(fileType type){
    if(type == 0) {
        auto uniqueP = make_unique<writerUMC>();
        return uniqueP;
    }
    if(type == 1){
        auto uniqueP = make_unique<writerSimple>();
        return uniqueP;
    }
    return nullptr;
}

int ParserXML::count = 0;

int main(int argc,const char *argv[]){
    string input;
    string outputFile;
    bool helpCalled = false;
    
    for(int i = 0; i < argc; i++){
        string stringInput = argv[i];
        cout << stringInput << endl;
        if(stringInput == "-i"){
            if(argc <= i+1 && !helpCalled) 
                throw invalid_argument("There must be an input file");
            input = argv[i+1];
        }
        if(stringInput == "-o" && argc > i+1)
            outputFile = argv[i+1];
        
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

    auto pXML = make_unique<ParserXML>(input);
    int folder;
    std::stringstream ss;
    outputFile = outputFile + pXML->SplitFilename(input) + "/";
    ss << outputFile;
    folder = mkdir(ss.str().c_str(), S_IRWXU | S_IRWXG);
    if(folder == 0) std::cout << "Created " << ss.str() << " success\n";
    int file;
    cout << "WHICH TYPE DO YOU WANT" << endl;
    cout << "0 : UMCFile" << endl;
    cout << "1 : SimpleFile " << endl;
    cin >> file;  
    if(file == 0){
        auto obj = writer::write(UMCFile);
        outputFile = outputFile  + "UMC/";
        cout << outputFile << endl;
        ss.str("");
        ss.clear();
        ss <<  outputFile;
        folder = mkdir(ss.str().c_str(), S_IRWXU | S_IRWXG);
        if(folder == 0) std::cout << "Created " << ss.str() << " success\n";
        cout << "HOW MANY TRAIN DO YOU WANT?" <<endl;
        cout << "PRESS:" << endl;
        cout << " 1 : ONE" << endl;
        cout << " 2 : TWO"  <<endl;
        int train;
        bool correct = false;
        int select;
        cin >> train;
        while(!correct){
            switch(train){
                case 1:{
                    correct = true;
                    int combined;
                    do{
                        cout << "DO YOU WANT TO COMBINE ROUTE WITH LENGTH < 4 ?" <<endl;
                        cout << "PRESS:" << endl;
                        cout << "0 : YES" << endl;
                        cout << "1 : NO"  <<endl;
                        cin >> combined;
                    }while( combined != 0 && combined != 1);
                    if(combined == 0){
                        outputFile = outputFile + "CombinedSingleRoutes/";
                        cout << outputFile << endl;
                        ss.str("");
                        ss.clear();
                        ss <<  outputFile;
                        folder = mkdir(ss.str().c_str(),S_IRWXU | S_IRWXG);
                        if(folder == 0) std::cout << "Created " << ss.str() << " success\n";
                        Interlock il_comb = pXML->getIl().routeCombiner(pXML->getIl());
                        pXML->setIl(il_comb);
                        obj->writeFile(outputFile,pXML,train,select);
                    }
                    else{
                        cout << "DO YOU WANT SELECT THE ROUTE?" <<endl;
                        cout << "1 : YES" <<endl;
                        cout << "2 : NO " <<endl;
                        cin >> select;
                        if(select != 1){
                            outputFile = outputFile + "singleRoute/";
                            cout << outputFile << endl;
                            ss.str("");
                            ss.clear();
                            ss <<  outputFile;
                            folder = mkdir(ss.str().c_str(), S_IRWXU | S_IRWXG);
                            if(folder == 0) std::cout << "Created " << ss.str() << " success\n";
                            obj->writeFile(outputFile,pXML,train,select);
                        }else{
                            int route1;
                            int size = (int)pXML->getIl().getRoutes().size();
                            do{
                                cout << "SELECT ROUTE 1 BETWEEN 0 AND " << size << " : "; 
                                cin >> route1;
                            }while(route1 < 0 && route1 >= size );
                            outputFile = outputFile + "singleRouteSelected/";
                            cout << outputFile << endl;
                            ss.str("");
                            ss.clear();
                            ss << outputFile;
                            folder = mkdir(ss.str().c_str(), S_IRWXU | S_IRWXG);
                            if(folder == 0) std::cout << "Created " << ss.str() << " success\n";
                            obj->writeFile(outputFile,pXML,train,select,route1);
                            
                        }
                    }
                }break;
                case 2: {
                    correct = true;
                    cout << "DO YOU WANT SELECT THE ROUTES?" <<endl;
                    cout << "1 : YES" <<endl;
                    cout << "2 : NO " <<endl;
                    cin >> select;
                    if(select != 1){
                        outputFile = outputFile + "DoubleRoute/";
                        cout << outputFile << endl;
                        ss.str("");
                        ss.clear(); 
                        ss << outputFile;
                        folder = mkdir(ss.str().c_str(), S_IRWXU | S_IRWXG);
                        if(folder == 0) std::cout << "Created " << ss.str() << " success\n";
                        obj->writeFile(outputFile,pXML,train,select);
                    }else{
                        int route1;
                        int route2;
                        int size = (int)pXML->getIl().getRoutes().size();
                        do{
                            cout << "SELECT ROUTE 1 BETWEEN 0 AND " << size << " : "; 
                            cin >> route1;
                        }while(route1 > size );
                        do{
                            cout << "SELECT ROUTE 2 BETWEEN 0 AND " << size << " : "; 
                            cin >> route2;
                        }while(route2 > size && route2 != route1);
                        outputFile = outputFile + "DoubleRouteSelected/";
                        cout << outputFile << endl;
                        ss.str("");
                        ss.clear();
                        ss <<  outputFile;
                        folder = mkdir(ss.str().c_str(), S_IRWXU | S_IRWXG);
                        if(folder == 0) std::cout << "Created " << ss.str() << " success\n";
                        obj->writeFile(outputFile,pXML,train,select,route1,route2);
                    }
                }break;
                default: 
                    cerr << "Number of trains incorrect. Please select '1' or '2'. " << endl;
                    cin >> train;
            }
        } 
    }
    if(file == 1){
        auto obj = writer::write(SimpleFile);
        outputFile = outputFile  + "Simple/";
        cout << outputFile << endl;
        ss.str("");
        ss.clear();
        ss <<  outputFile;
        folder = mkdir(ss.str().c_str(), S_IRWXU | S_IRWXG);
        if(folder == 0) std::cout << "Created " << ss.str() << " success\n";
        obj->writeFile(outputFile,pXML);
    }
    
}

