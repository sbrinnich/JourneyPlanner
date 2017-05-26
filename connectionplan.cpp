/* Copyright Alexander Dietrich und Selina Brinnich */

#include "connectionplan.h"

Connection::Connection(std::string destination, std::string line, int traveltime)
        : destination(destination), line(line), traveltime(traveltime) {}

ConnectionPlan::ConnectionPlan() {}

ConnectionPlan::~ConnectionPlan() {
    for (auto it = stations.begin(); it != stations.end(); ++it ){
        for(unsigned int i = 0; i < it->second.size(); i++){
            delete it->second.at(i);
            it->second.erase(it->second.begin()+i);
        }
        stations.erase(it->first);
    }
}

void ConnectionPlan::readFromFile(std::string filepath) {
    // TODO
}

void ConnectionPlan::addConnection(std::string first_station, std::string second_station, std::string line,
                                   int traveltime) {

    if(first_station == second_station) return;

    // Check if nodes already exist
    if(stations.count(first_station) == 0){
        // New station found
        stations.insert({first_station, std::vector<Connection*>()});
    }
    if(stations.count(second_station) == 0){
        // New station found
        stations.insert({second_station, std::vector<Connection*>()});
    }

    // Add connections
    // TODO? Check if connections already exist
    stations[first_station].push_back(new Connection(second_station, line, traveltime));
    stations[second_station].push_back(new Connection(first_station, line, traveltime));
}

std::vector<Connection*> ConnectionPlan::getShortestPath(std::string start_station, std::string end_station) {
    // List of visited nodes

    return std::vector<Connection*>();
}

void ConnectionPlan::printPath(std::vector<Connection *> path) {
    // TODO
    std::cout << "Printing!" << std::endl;
}