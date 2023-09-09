# Railway ParserXML 

### Description 
Text-based markup language called XML (eXtensible Markup Language)
offers a flexible and organized approach to express data. It is both human
and machine readable and it is structured as a hierarchical tree of elements.
The railway XML used in this work consist of a main root called "interlocking" that is divided in "network" and "routetable".

Link to the whole [paper](Implementation_of_a_parser_for_railway_interlockings.pdf)

### Dependencies
  * [WxWidgets](https://github.com/wxWidgets/wxWidgets)
  * [Nlohmann/json](https://github.com/nlohmann/json)
  * [RapidXML](https://rapidxml.sourceforge.net/)

### Pre-requisites
Hbase, Storm and Hadoop have to be installed and set correctly on your pseudo-distribuited cluster.

### Usage
Launch the task "Compile and Link" to generate the .exe file into the exec folder.


After that, you can finally execute the parser:
* `Insert a valid RailwayXML and a valid output folder`
  * Select the configuration you would like to test
    *  Calculate the roots to choose your path to test
  * DrawMe button will create a graph representing the topology of the interlocking
  * Run will execute the algorithm processing the XML file. In output you will have the configurations chosen in UMC plus detailed info, the topology in .json file and the image of the topology.


### Tips
This whole project was executed on Windows using VisualStudio code.

