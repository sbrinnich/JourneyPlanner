/* Copyright Alexander Dietrich und Selina Brinnich */

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "connectionplan.h"

void printUsage(){
    std::cout << std::endl << "Usage: JourneyPlanner [filename_for_transport_network]" << std::endl << std::endl;
}

int main(int argc, char *argv[]) {

    // Check if filepath was entered as program argument
    if(argc != 2){
        std::cout << "Invalid parameters specified!" << std::endl;
        printUsage();
        return -1;
    }

    // Check if file exists
    std::string filename = argv[1];
    if(!std::ifstream(filename)){
        std::cout << "Specified file does not exist or could not be read!" << std::endl;
        printUsage();
        return -1;
    }

    std::cout << "Welcome to JourneyPlanner!" << std::endl;
    ConnectionPlan* plan = new ConnectionPlan();

    // Read connection plan from file
    plan->readFromFile(filename);

    std::string in = "", station1 = "", station2 = "";
    int time = 0;
    bool exit = false;
    do{
        std::cout << std::endl << "Press s for searching a path, c for setting the time for changing lines "
                "or any other character to exit the program!" << std::endl;
        std::cin >> in;
        if(in.compare("s") == 0){
            // Clear cin for getline
            std::cin.clear();
            std::cin.sync();
            // Get starting point
            std::cout << "Where do you want to start your journey?";
            std::getline( std::cin, station1);
            // Get destination point
            std::cout << "What is your destination?";
            std::getline( std::cin, station2);
            std::cout << std::endl;
            // Calculate and print path
            plan->getShortestPath(station1, station2);
        }else if(in.compare("c") == 0){
            // Get new time
            std::cout << "How long (in minutes) do you need for changing lines?";
            std::cin >> in;
            std::cout << std::endl;
            // Parse into integer
            std::stringstream stream(in);
            stream >> time;
            if(!stream.fail()){
                // Try to set new time
                bool set = plan->setChangingTime(time);
                if(set){
                    std::cout << "New time was set successfully!" << std::endl;
                }else {
                    std::cout << "Input could not be parsed! Please try again!" << std::endl;
                }
            }else{
                // Parse not successful
                std::cout << "Input could not be parsed! Please try again!" << std::endl;
            }
        }else{
            // Exit program
            exit = true;
        }
    }while(!exit);

    // Clean up
    delete plan;
    return 0;
}