#include "writerUMC.hpp"
#include "ParserXML.hpp"
#include <string>
#include <ctype.h>
#include <algorithm>

// void writerUMC::writeFile(string outputFile,NetworkLayout nl,Interlock il,map<int,string> pl,map<int,string> mb,int choose){
void writerUMC::writeFile(const string outputFile, unique_ptr<ParserXML> &pXML, int train, int select, int route1, int route2)
{
    // 1. create a file.txt for each route and add an extra routes that continue
    // 2. create a file.txt that contents all routes
    // 3. create a file.txt that contents some routes
    pXML->getIl().generateMaxChunk();
    if (select == 0)
    {
        for (int firstRoute = 0; firstRoute < (int)pXML->getIl().getRoutes().size(); firstRoute++)
        {
            if (train == 0)
            {
                string outputFiletxt = outputFile + "route" + to_string(pXML->getIl().getRoutes().at(firstRoute).getRouteId()+1) + ".txt";
                try
                {
                    ofstream myfile;
                    myfile.open(outputFiletxt);
                    myfile << "\n/* NetworkLayout */\n\n";
                    myfile << pXML->getNl().toStringAdaptive(pXML->getIl().getRoutes().at(firstRoute), pXML->getPlCorrispondence(), pXML->getMbCorrispondence());
                    myfile << "\n\n/* NetworkLayout End */\n\n";
                    myfile << "\n/* Interlocking */\n\n";
                    myfile << pXML->getIl().getRoutes().at(firstRoute).toString(pXML->getIl().getMaxPathLength(), pXML->getIl().getMaxChunk()) + "\n";
                    myfile << "\n/* Interlocking End */\n";
                    myfile << "\n/* UMC code */\n";
                    myfile << codeUMC;
                    myfile << defaultUMCsetupOneRoute(pXML, firstRoute);
                    myfile.close();
                    cout << "Successfully wrote to the file." << endl;
                }
                catch (std::ofstream::failure &e)
                {
                    std::cerr << e.what() << std::endl;
                }
            }
            else if (train == 1)
            {
                int secondRoute = pXML->getSecondRoute(firstRoute);
                string outputFiletxt = outputFile + "route" + stringCombinerId(pXML, firstRoute, secondRoute) + ".txt";
                try
                {
                    ofstream myfile;
                    myfile.open(outputFiletxt);
                    myfile << "\n/* NetworkLayout */\n\n";
                    myfile << stringCombinerNl(pXML, firstRoute, secondRoute);
                    myfile << "\n\n/* NetworkLayout End */\n\n";
                    myfile << "\n/* Interlocking */\n\n";
                    myfile << stringCombinerIl(pXML, firstRoute, secondRoute) + "\n";
                    myfile << "\n/* Interlocking End */\n";
                    myfile << "\n/* UMC code */\n";
                    myfile << codeUMC;
                    // myfile << defaultUMCsetupOneRoute(pXML->getNl(),pXML->getIl(),i,pXML->getPlCorrispondence(),pXML->getMbCorrispondence());
                    myfile << defaultUMCsetupTwoRoute(pXML, firstRoute, secondRoute);
                    myfile.close();
                    cout << "Successfully wrote to the file." << endl;
                }
                catch (std::ofstream::failure &e)
                {
                    std::cerr << e.what() << std::endl;
                }
            }
        }
    }
    else if ( select == 1)
    {
        if (train == 0)
        {
            // check if you want print at display
            string outputFiletxt = outputFile + "route" + to_string(pXML->getIl().getRoutes().at(route1).getRouteId()+1) + ".txt";
            try
            {
                ofstream myfile;
                myfile.open(outputFiletxt);
                myfile << "\n/* NetworkLayout */\n\n";
                myfile << pXML->getNl().toStringAdaptive(pXML->getIl().getRoutes().at(route1), pXML->getPlCorrispondence(), pXML->getMbCorrispondence());
                myfile << "\n\n/* NetworkLayout End */\n\n";
                myfile << "\n/* Interlocking */\n\n";
                myfile << pXML->getIl().getRoutes().at(route1).toString(pXML->getIl().getMaxPathLength(), pXML->getIl().getMaxChunk()) + "\n";
                myfile << "\n/* Interlocking End */\n";
                myfile << "\n/* UMC code */\n";
                myfile << codeUMC;
                myfile << defaultUMCsetupOneRoute(pXML, route1);
                myfile.close();
                cout << "Successfully wrote to the file." << endl;
            }
            catch (std::ofstream::failure &e)
            {
                std::cerr << e.what() << std::endl;
            }
        }
        else if ( train == 1)
        {
            string outputFiletxt = outputFile + "route" + stringCombinerId(pXML, route1, route2) + ".txt";
            try
            {
                ofstream myfile;
                myfile.open(outputFiletxt);
                myfile << "\n/* NetworkLayout */\n\n";
                myfile << stringCombinerNl(pXML, route1, route2);
                myfile << "\n\n/* NetworkLayout End */\n\n";
                myfile << "\n/* Interlocking */\n\n";
                myfile << stringCombinerIl(pXML, route1, route2) + "\n";
                myfile << "\n/* Interlocking End */\n";
                myfile << "\n/* UMC code */\n";
                myfile << codeUMC;
                myfile << defaultUMCsetupTwoRoute(pXML, route1, route2);
                myfile.close();
                cout << "Successfully wrote to the file." << endl;
            }
            catch (std::ofstream::failure &e)
            {
                std::cerr << e.what() << std::endl;
            }
        }
    }
}

string writerUMC::defaultUMCsetupOneRoute(unique_ptr<ParserXML> &pXML, int firstRoute)
{
    string s;
    s += "Objects:\n\n";
    // In UMC I can't insert object that I didn't declare. So:
    // 1 - I use only the pathChunck
    // 2 - I declare everything (it may be too big)
    s += signalObjectUmcOneRoute(pXML->getIl().getRoutes().at(firstRoute), pXML->getPlCorrispondence(), pXML->getMbCorrispondence(), pXML->getNl());
    s += pointObjectUmcOneRoute(pXML->getIl().getRoutes().at(firstRoute), pXML->getPlCorrispondence(), pXML->getNl()); // changed
    s += linearObjectUmcOneRoute(pXML->getIl().getRoutes().at(firstRoute), 1, pXML->getPlCorrispondence(), pXML->getMbCorrispondence(), pXML->getNl());
    s += trainObjectUmcOneRoute(pXML->getIl().getRoutes().at(firstRoute), 1, pXML->getPlCorrispondence());
    s += abstractionUmcOneRoute(pXML->getIl().getRoutes().at(firstRoute), 1, pXML->getPlCorrispondence(), pXML->getMbCorrispondence(), pXML->getNl());
    return s;
}

string writerUMC::defaultUMCsetupTwoRoute(unique_ptr<ParserXML> &pXML, int firstRoute, int secondRoute)
{
    string s;
    s += "Objects:\n\n";
    s += signalObjectUmcOneRoute(pXML->getIl().getRoutes().at(firstRoute), pXML->getPlCorrispondence(), pXML->getMbCorrispondence(), pXML->getNl());
    s += signalObjectUmcOneRoute(pXML->getIl().getRoutes().at(secondRoute), pXML->getPlCorrispondence(), pXML->getMbCorrispondence(), pXML->getNl());

    s += pointObjectUmcTwoRoute(pXML->getIl().getRoutes().at(firstRoute), pXML->getIl().getRoutes().at(secondRoute), pXML->getPlCorrispondence(), pXML->getNl());

    s += linearObjectUmcOneRoute(pXML->getIl().getRoutes().at(firstRoute), 1, pXML->getPlCorrispondence(), pXML->getMbCorrispondence(), pXML->getNl());
    s += linearObjectUmcOneRoute(pXML->getIl().getRoutes().at(secondRoute), 2, pXML->getPlCorrispondence(), pXML->getMbCorrispondence(), pXML->getNl());

    s += trainObjectUmcOneRoute(pXML->getIl().getRoutes().at(firstRoute), 1, pXML->getPlCorrispondence());
    s += trainObjectUmcOneRoute(pXML->getIl().getRoutes().at(secondRoute), 2, pXML->getPlCorrispondence());

    s += abstractionUmcTwoRoute(pXML->getIl().getRoutes().at(firstRoute), pXML->getIl().getRoutes().at(secondRoute), 1, 2, pXML->getPlCorrispondence(), pXML->getMbCorrispondence(), pXML->getNl());
    return s;
}

string writerUMC::linearObjectUmcOneRoute(Route route, int number, const map<int, string> plC, const map<int, string> sC, NetworkLayout nl)
{
    string output;
    int index;
    for (int i = 0; i < (int)route.getPath().size(); i++)
    {
        if (route.getPath().at(i) >= (int)route.getPoints().size() and i != (int)route.getPath().size() - 1)
        {
            index = (int)route.getPath().at(i) - nl.getPoints().size();
            // I did that to put NULL on last linear and stop the request. I know that there is also the point that have to checked
            string up = nl.getLinears().at(index).getUpNeig() != -1 ? plC.find(nl.getLinears().at(index).getUpNeig())->second : "null";
            string down = nl.getLinears().at(index).getDownNeig() != -1 ? plC.find(nl.getLinears().at(index).getDownNeig())->second : "null";
            string sign = findMb(route, nl, nl.getLinears().at(index).sectionId, sC);
            string train = (nl.getLinears().at(index).sectionId == route.getPath().at(0)) ? "train" + to_string(number) : "null";
            output += plC.find(nl.getLinears().at(index).sectionId)->second + ": Linear (\n\t";
            output += "prev => [";
            // output += i != 0 ? (route.getDirection() == "up" ? down : up) : "null";
            if(number == 2)
                output += "null,";
            if (i != 0)
                output += route.getDirection() == "up" ? down : up;
            else
                output += "null";
            output += "],\n\t";
            output += "next => [";
            if(number == 2)
                output += "null,";
            output += (route.getDirection() == "up" ? up : down) + "],\n\t";
            output += "sign => [" ;
            if(number == 2)
                output += "null,";
            output += sign + "],\n\t";
            output += "train => " + train + "\n);\n\n";
        }
        else if (i == (int)route.getPath().size() - 1)
        {
            index = (int)route.getPath().at(i) - nl.getPoints().size();
            string up = nl.getLinears().at(index).getUpNeig() != -1 ? plC.find(nl.getLinears().at(index).getUpNeig())->second : "null";
            string down = nl.getLinears().at(index).getDownNeig() != -1 ? plC.find(nl.getLinears().at(index).getDownNeig())->second : "null";
            string sign = findMb(route, nl, nl.getLinears().at(index).sectionId, sC);
            string train = (nl.getLinears().at(index).sectionId == route.getPath().at(0)) ? "train" + number : "null";
            output += plC.find(nl.getLinears().at(index).sectionId)->second + ": Linear (\n\t";
            output += "prev => [";
            if(number == 2)
                output += "null,";
            output += (route.getDirection() == "up" ? down : up) + "],\n\t";
            output += "next => [";
            if(number == 2)
                output += "null,";
            output += "null],\n\t";
            output += "sign => [";
            if(number == 2)
                output += "null,";
            output += sign + "],\n\t";
            output += "train => " + train + "\n);\n\n";
        }
    }
    return output;
}

// USED TO FIND THE MARKERBOARD ON THE CHUNK
string writerUMC::findMb(Route route, NetworkLayout nl, int linearId, const map<int, string> sC)
{
    for (int i = 0; i < (int)route.getSignals().size(); i++)
    {
        if (route.getSignals().at(i) == true &&
            nl.getSignals().at(i).getSectionId() == linearId &&
            nl.getSignals().at(i).getLinearEnd() == route.getDirection())
        {
            return sC.find(nl.getSignals().at(i).getMbId())->second;
        }
    }
    return "null";
}

string writerUMC::pointObjectUmcOneRoute(Route route, const map<int, string> plC, NetworkLayout nl)
{
    string output;
    for (int i = 0; i < (int)route.getPath().size(); i++)
    {
        int current = route.getPath().at(i);
        if (current < (int)route.getPoints().size() && route.getPoints().at(current) != "INTER")
        {
            string name = plC.find(current)->second;
            output += (isdigit(name[0]) ? ("_" + name) : name) + ": Point (\n\t";
            output += "prev => [" + plC.find(route.getPath().at(i - 1))->second + "],";
            output += "\n\t";
            output += "next => [" + plC.find(route.getPath().at(i + 1))->second + "],";
            output += "\n\t";
            string conf = (route.getPoints().at(current) == "PLUS") ? "true" : "false";
            output += "conf => [" + conf + "],\n\t";
            output += "train => null\n);\n\n";
        }
    }
    // Chain effect probably..
    if (output.empty())
    {
        // string prev,next;
        for (int i = 0; i < (int)route.getPoints().size(); i++)
        {
            if (route.getPoints().at(i) != "INTER")
            {
                string name = plC.find(nl.getPoints().at(i).sectionId)->second;
                output += isdigit(name[0]) ? "_" + name : name + ": Point (\n\t";
                output += "prev => [null],\n\t";
                output += "next => [null],\n\t";
                string conf = route.getPoints().at(i) == "PLUS" ? "true" : "false";
                output += "conf => [" + conf + "],\n\t";
                output += "train => null\n);\n\n";
            }
        }
    }
    return output;
}

string writerUMC::signalObjectUmcOneRoute(Route route, const map<int, string> plC, const map<int, string> sC, NetworkLayout nl)
{
    string output;
    int index;
    for (int i = 0; i < (int)route.getPath().size(); i++)
    {
        if (route.getPath().at(i) >= (int)route.getPoints().size())
        {
            index = route.getPath().at(i) - nl.getPoints().size();
            int linId = nl.getLinears().at(index).sectionId;
            string sign = findMb(route, nl, linId, sC);
            output += sign + " : Signal(\n\t";
            output += "linear => " + plC.find(route.getPath().at(i))->second + "\n);\n\n";
        }
    }
    return output;
}

string writerUMC::trainObjectUmcOneRoute(Route route, int number, const map<int, string> plC)
{
    string output;
    output += "train" + to_string(number) + " : Train (\n\t";
    output += "id_itinerary => ";
    output += (number == 1)  ? "0,\n\t" : "1,\n\t";
    output += "route_nodes => [";
    for (int i = 0; i < (int)route.getPath().size(); i++)
    {
        output += plC.find(route.getPath().at(i))->second + ", ";
    }
    output = output.substr(0, output.length() - 2);
    output += "],\n\t";
    output += "node => " + plC.find(route.getPath().at(0))->second + "\n";
    output += ");\n";
    return output;
}

string writerUMC::derailAbsOneRoute(Route route, int number, const map<int, string> plC, NetworkLayout nl)
{
    string output;
    bool write = false;
    for (int i = 0; i < (int)route.getPath().size(); i++)
    {
        int current = route.getPath().at(i);
        if (current < (int)route.getPoints().size())
        {
            string point = plC.find(current)->second;
            output += "State : " + point + ".conf[0] == ";
            output += (route.getPoints().at(current) == "PLUS") ? "false" : "true";
            output += " and " + point + ".train == train" + to_string(number);
            output += " and inState(train" + to_string(number) + ".MOVE) -> DERAIL_train" + to_string(number) + "\n\t";
            write = true;
        }
    }
    if (write == false)
    {
        for (int i = 0; i < (int)route.getPoints().size(); i++)
        {
            if (route.getPoints().at(i) != "INTER")
            {
                string point = plC.find(nl.getPoints().at(i).getSectionId())->second;
                output += "State : " + point + ".conf[0] == ";
                output += route.getPoints().at(i) == "PLUS" ? "false" : "true";
                output += " -> POSSIBLE_DERAIL\n\t";
            }
        }
    }
    return output;
}

string writerUMC::brokenSignalsOneRoute(Route route, const int number, const map<int, string> plC, const map<int, string> sC, NetworkLayout nl)
{
    string output;
    for (int i = 0; i < (int)route.getPath().size(); i++)
    {
        if (route.getPath().at(i) >= (int)route.getPoints().size())
        {
            int index = (int)route.getPath().at(i) - nl.getPoints().size();
            output += "State : inState(train" + to_string(number) + ".MOVE) and ";
            int linId = nl.getLinears().at(index).sectionId;
            string sign = findMb(route, nl, linId, sC);
            output += sign + ".red == true";
            string init = plC.find(route.getPath().at(i))->second;
            output += " and " + init + ".train == train" + to_string(number) + " -> FAULT_" + sign;
            output += "\n\t";
        }
    }
    return output;
}

string writerUMC::abstractionUmcOneRoute(Route route, int number, const map<int, string> plC, const map<int, string> sC, const NetworkLayout nl)
{
    string output = "Abstractions{\n\t";
    output += "Action $1($*) -> $1($*)\n\t";
    output += "State : train" + to_string(number) + ".node == " + plC.find(route.getPath().back())->second + " -> train" + to_string(number) + "_arrived\n\t";
    output += derailAbsOneRoute(route, number, plC, nl);
    output += brokenSignalsOneRoute(route, number, plC, sC, nl);
    output += "\n}";
    return output;
}
string writerUMC::abstractionUmcTwoRoute(const Route &route1,const Route& route2, int n1, int n2, const map<int, string> plC, const map<int, string> sC, const NetworkLayout nl)
{
    string output = "Abstractions{\n\t";
    output += "Action $1($*) -> $1($*)\n\t";
    output += trainArrived(route1, n1, plC);
    output += trainArrived(route2, n2, plC);
    output += derailAbsOneRoute(route1, n1, plC, nl);
    output += derailAbsOneRoute(route2, n2, plC, nl);
    output += brokenSignalsOneRoute(route1, n1, plC, sC, nl);
    output += brokenSignalsOneRoute(route2, n2, plC, sC, nl);
    output += "\n}";
    return output;
}
string writerUMC::trainArrived(Route route, int number, const map<int, string> plC) const
{
    string output;
    output += "State : train" + to_string(number) + ".node == " + plC.find(route.getPath().back())->second + " -> train" + to_string(number) + "_arrived\n\t";
    ;
    return output;
}

string writerUMC::stringCombinerNl(unique_ptr<ParserXML> &pXML, int i, int j) const
{
    string s;
    s += pXML->getNl().toStringAdaptive(pXML->getIl().getRoutes().at(i), pXML->getPlCorrispondence(), pXML->getMbCorrispondence());
    s += "\n\n";
    s += pXML->getNl().toStringAdaptive(pXML->getIl().getRoutes().at(j), pXML->getPlCorrispondence(), pXML->getMbCorrispondence());
    return s;
}

string writerUMC::stringCombinerIl(unique_ptr<ParserXML> &pXML, int i, int j) const
{
    string s;
    s += pXML->getIl().getRoutes().at(i).toString(pXML->getIl().getMaxPathLength(), pXML->getIl().getMaxChunk());
    s += "\n\n";
    s += pXML->getIl().getRoutes().at(j).toString(pXML->getIl().getMaxPathLength(), pXML->getIl().getMaxChunk());
    return s;
}

string writerUMC::stringCombinerId(unique_ptr<ParserXML> &pXML, int i, int j) const
{
    string s;
    s += to_string(pXML->getIl().getRoutes().at(i).getRouteId()+1) + "-";
    s += to_string(pXML->getIl().getRoutes().at(j).getRouteId()+1);
    return s;
}

string writerUMC::pointObjectUmcTwoRoute(Route &route1, Route &route2, const map<int, string> plC, NetworkLayout nl) const
{
    string output;
    vector<int> p1 = route1.getPath();
    vector<int> p2 = route2.getPath();
    bool find1, find2;
    int index1, index2;
    for (int i = 0; i < (int)nl.getPoints().size(); i++)
    {
        if (route1.getPoints().at(i) != "INTER" || route2.getPoints().at(i) != "INTER")
        {
            for (int j = 0; j < (int)route1.getPath().size(); j++)
            {
                if (route1.getPath().at(j) == i)
                {
                    find1 = true;
                    index1 = j;
                }
            }
            for (int w = 0; w < (int)route2.getPath().size(); w++)
            {
                if (route2.getPath().at(w) == i)
                {
                    find2 = true;
                    index2 = w;
                }
            }
            string name = plC.find(i)->second;
            output += (isdigit(name[0]) ? ("_" + name) : name) + ": Point (\n\t";
            string conf1 = (route1.getPoints().at(i) == "PLUS") ? "true" : "false";
            string conf2 = (route2.getPoints().at(i) == "PLUS") ? "true" : "false";
            if ((find1 != true) && (find2 != true))
            {
                output += "prev => [null,null],";
                output += "\n\t";
                output += "next => [null,null],";
                output += "\n\t";
                output += "conf => [" + conf1 + "," + conf2 + "],\n\t";
                output += "train => null\n);\n\n";
            }
            else if ((find1 == true) && (find2 != true))
            {
                string prova = plC.find(p1.at(index1 - 1))->second;
                output += "prev => [" + prova + ",null],";
                output += "\n\t";
                output += "next => [" + plC.find(p1.at(index1 + 1))->second + ",null],";
                output += "\n\t";
                output += "conf => [" + conf1 + "," + conf2 + "],\n\t";
                output += "train => null\n);\n\n";
            }
            else if ((find1 != true) && (find2 == true))
            {
                string prova = plC.find(p2.at(index2 - 1))->second;
                output += "prev => [null," + prova + "],";
                output += "\n\t";
                output += "next => [null," + plC.find(p2.at(index2 + 1))->second + "],";
                output += "\n\t";
                output += "conf => [" + conf1 + "," + conf2 + "],\n\t";
                output += "train => null\n);\n\n";
            }
            else
            {
                output += "prev => [" + plC.find(p1.at(index1 - 1))->second + "," + plC.find(p2.at(index2 - 1))->second + "],";
                output += "\n\t";
                output += "next => [" + plC.find(p1.at(index1 + 1))->second + "," + plC.find(p2.at(index2 + 1))->second + "],";
                output += "\n\t";
                output += "conf => [" + conf1 + "," + conf2 + "],\n\t";
                output += "train => null\n);\n\n";
            }
        }
    }
    return output;
}



