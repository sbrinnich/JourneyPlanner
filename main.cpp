/* Copyright Alexander Dietrich und Selina Brinnich */

#include <iostream>
#include <fstream>
#include <string>
#include "connectionplan.h"

int main(int argc, char *argv[]) {

    // Check if filepath was entered as program argument
    if(argc != 2){
        std::cout << "Invalid parameters specified!" << std::endl;
        return -1;
    }

    // Check if file exists
    std::string filename = argv[1];
    if(!std::ifstream(filename)){
        std::cout << "Specified file does not exist or could not be read!" << std::endl;
        return -1;
    }

    std::cout << "Welcome to JourneyPlanner!" << std::endl;
    ConnectionPlan* plan = new ConnectionPlan();

    // Read connection plan from file
    plan->readFromFile(filename);

    std::string in = "", station1 = "", station2 = "";
    do{
        std::cout << std::endl << "Press s for searching a path or any other character to exit program!" << std::endl;
        std::cin >> in;
        if(in.compare("s") == 0){
            std::cin.clear();
            std::cin.sync();
            std::cout << "Where do you want to start your journey?";
            std::getline( std::cin, station1);
            std::cout << "What is your destination?";
            std::getline( std::cin, station2);
            std::cout << std::endl;
            plan->getShortestPath(station1, station2);
        }
    }while(in.compare("s") == 0);

    delete plan;
    return 0;
}