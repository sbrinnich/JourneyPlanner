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
    }
    stations.clear();
}

void ConnectionPlan::readFromFile(std::string filepath) {
    std::string line = "", metro="", read = "", laststation = "";
    int traveltime = 0;
    bool stationnext;
    std::ifstream file(filepath, std::ios::in);
    while (!file.eof()) {
        getline(file, line, '\n');
        if(line.length() > 0){
            std::istringstream stream(line);
            getline(stream, metro, ':');
            stationnext = true;
            traveltime = 0;
            laststation = "";
            while(!stream.eof()){
                if(stationnext){
                    getline(stream, read, '"');
                    getline(stream, read, '"');
                    if(laststation.length() > 0){
                        addConnection(laststation, read, metro, traveltime);
                    }
                    laststation = read;
                }else{
                    getline(stream, read, ' ');
                    getline(stream, read, ' ');
                    std::stringstream intstream(read);
                    intstream >> traveltime;
                }
                stationnext = !stationnext;
            }
        }
    }
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
    // TODO Maybe? Check if connections already exist
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

void ConnectionPlan::printAdjacencyList(){
    for (auto it = stations.begin(); it != stations.end(); ++it ){
        std::cout << "Station: " << it->first << std::endl;
        for(unsigned int i = 0; i < it->second.size(); i++){
            std::cout << it->second.at(i) << std::endl;
            delete it->second.at(i);
            it->second.erase(it->second.begin()+i);
        }
    }
}