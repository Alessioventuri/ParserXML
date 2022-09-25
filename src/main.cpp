#include "NetworkLayout.hpp"
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

int main(int argc,const char *argv[]){
    // bool outputToFile = false;
    // string input;
    string input = "/Users/alessioventuri/Desktop/XML/lvr_7_right_rt.xml";
    // string outputFile;
    string outputFile = "/Users/alessioventuri/Desktop/Interlocking/";
    // bool helpCalled = false;
    
    // for(int i = 0; i < argc; i++){
    //     string stringInput = argv[i];
    //     cout << stringInput << endl;
    //     if(stringInput == ("-i")){
    //         if(argc <= i+1 && !helpCalled) 
    //             throw invalid_argument("There must be an input file");
    //         input = argv[i+1];
    //     }
    //     if(stringInput == "-o"){
    //         if(argc >i+1)
    //             outputFile = argv[i+1];
    //     }
    //     if(stringInput == "-h"){
    //         helpCalled = true;
    //         break;
    //     }
    // }
    // if(input == "" && !helpCalled)
    //     throw invalid_argument("There must be an input file");
    // if (helpCalled){
    //     cout << "-i <inputfile> ";
    //     cout << "Provides the input xml file that should be parsed" << endl;
    //     cout << "-o <outputfile> ";
    //     cout << "Provides the output text which the program should write the config data"<<endl;
    //     cout << "This remove the printout in the terminal and is optional"<< endl;
    //     cout << "-h ";
    //     cout << "Shows this info" << endl;

    //     exit(0);

    // }

    ParserXML *pXML = new ParserXML(input);
    int rc;
    std::stringstream ss;
    cout << outputFile << endl; 
    outputFile = outputFile + (pXML)->SplitFilename(input) + "/";
    cout << outputFile << endl;
    ss << outputFile;
    rc = mkdir(ss.str().c_str(), 0777);
    if(rc == 0) std::cout << "Created " << ss.str() << " success\n";
    int file;
    cout << "WHICH TYPE DO YOU WANT" << endl;
    cout << "0 : UMCFile" << endl;
    cout << "1 : SimpleFile " << endl;
    cin >> file;
    // outputFile = outputFile + "SingleRoute/";
    // writer* obj = writer::write(UMCFileOneRoute);
    // cout << outputFile << endl;
    // std::stringstream sss;
    // sss <<  outputFile;
    // int rc1 = mkdir(sss.str().c_str(), 0777);
    // if(rc1 == 0) std::cout << "Created " << sss.str() << " success\n";
    // //Interlock il_comb = il.routeCombiner(nl,il);
    // obj->writeFile(outputFile,pXML,1);
    // }
    writer* obj;
    if(file == 0){
        outputFile = outputFile  + "UMC/";
        cout << outputFile << endl;
        ss.str("");
        ss.clear();
        ss <<  outputFile;
        int rc1 = mkdir(ss.str().c_str(), 0777);
        if(rc1 == 0) std::cout << "Created " << ss.str() << " success\n";
        cout << "HOW MANY TRAIN DO YOU WANT?" <<endl;
        cout << "PRESS:" << endl;
        cout << " 1 : ONE" << endl;
        cout << " 2 : TWO"  <<endl;
        int train;
        bool correct = false;
        int select;
        cin >> train;
        while(!correct){
            if(train == 1){
                correct = true;
                obj = writer::write(UMCFile);
                int combined;
                cout << "DO YOU WANT TO COMBINE ROUTE WITH LENGTH < 4 ?" <<endl;
                cout << "PRESS:" << endl;
                cout << "0 : YES" << endl;
                cout << "1 : NO"  <<endl;
                cin >> combined;
                if(combined == 0){
                    outputFile = outputFile + "CombinedSingleRoutes/";
                    cout << outputFile << endl;
                    ss.str("");
                    ss.clear();
                    ss <<  outputFile;
                    rc = mkdir(ss.str().c_str(), 0777);
                    if(rc == 0) std::cout << "Created " << ss.str() << " success\n";
                    Interlock il_comb = pXML->getIl().routeCombiner(pXML->getNl(),pXML->getIl());
                    pXML->setIl(il_comb);
                    // cout << il_comb.toStringCombiner() << endl;
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
                        rc = mkdir(ss.str().c_str(), 0777);
                        if(rc == 0) std::cout << "Created " << ss.str() << " success\n";
                        obj->writeFile(outputFile,pXML,train,select);
                    }else{
                        int route1;
                        int size = pXML->getIl().getRoutes().size();
                        do{
                            cout << "SELECT ROUTE 1 BETWEEN 0 AND " << size << " : "; 
                            cin >> route1;
                        }while(route1 < 0 and route1 >= size );
                        outputFile = outputFile + "singleRouteSelected/";
                        cout << outputFile << endl;
                        ss.str("");
                        ss.clear();
                        ss << outputFile;
                        rc = mkdir(ss.str().c_str(), 0777);
                        if(rc == 0) std::cout << "Created " << ss.str() << " success\n";
                        obj->writeFile(outputFile,pXML,train,select,route1);
                        
                    }
                }
            }else if(train == 2){
                correct = true;
                cout << "DO YOU WANT SELECT THE ROUTE?" <<endl;
                cout << "1 : YES" <<endl;
                cout << "2 : NO " <<endl;
                cin >> select;
                if(select != 1){
                    obj = writer::write(UMCFile);
                    outputFile = outputFile + "DoubleRoute/";
                    cout << outputFile << endl;
                    ss.str("");
                    ss.clear(); 
                    ss << outputFile;
                    rc = mkdir(ss.str().c_str(), 0777);
                    if(rc == 0) std::cout << "Created " << ss.str() << " success\n";
                    obj->writeFile(outputFile,pXML,train,select);
                }else{
                    int route1,route2;
                    int size = pXML->getIl().getRoutes().size();
                    do{
                        cout << "SELECT ROUTE 1 BETWEEN 0 AND " << size << " : "; 
                        cin >> route1;
                    }while(route1 > size );
                    do{
                        cout << "SELECT ROUTE 2 BETWEEN 0 AND " << size << " : "; 
                        cin >> route2;
                    }while(route2 > size );
                    
                    obj = writer::write(UMCFile);
                    outputFile = outputFile + "DoubleRouteSelected/";
                    cout << outputFile << endl;
                    ss.str("");
                    ss.clear();
                            ss <<  outputFile;
                    rc = mkdir(ss.str().c_str(), 0777);
                    if(rc == 0) std::cout << "Created " << ss.str() << " success\n";
                    obj->writeFile(outputFile,pXML,train,select,route1,route2);
                }
            }
            else{
                cerr << "Number of trains incorrect. Please select '1' or '2'. " << endl;
                cin >> train;

            }
        } 
    }
    if(file == 1){
        obj = writer::write(SimpleFile);
        outputFile = outputFile  + "Simple/";
        cout << outputFile << endl;
        ss.str("");
        ss.clear();
        ss <<  outputFile;
        rc = mkdir(ss.str().c_str(), 0777);
        if(rc == 0) std::cout << "Created " << ss.str() << " success\n";
        obj->writeFile(outputFile,pXML);
    }
    delete obj;
}
