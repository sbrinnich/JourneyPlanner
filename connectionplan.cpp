/* Copyright Alexander Dietrich und Selina Brinnich */

#include "connectionplan.h"

HeapNode::HeapNode(Connection* connection, int weight, Connection* lastconnection) :
        connection(connection), weight(weight), lastconnection(lastconnection) {}

Connection::Connection(std::string destination, std::string line, int traveltime)
        : destination(destination), line(line), traveltime(traveltime) {}

const std::string Connection::getDestination() {
    return destination;
}

const std::string Connection::getLine() {
    return line;
}

const int Connection::getTraveltime() {
    return traveltime;
}

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

const void ConnectionPlan::getShortestPath(std::string start_station, std::string end_station) {
    // List of visited nodes
    std::vector<std::string> visited;
    // Heap
    std::priority_queue<HeapNode, std::vector<HeapNode>, CompareHeapNode> heap;
    // Map for storing last connection for every node
    std::map<std::string,Connection*> lastcons;

    // Init variables
    int mintime=0;
    // Start search from destination
    std::string currentnode = end_station;
    Connection* lastconnection = nullptr;

    do {
        // Set current node visited
        visited.push_back(currentnode);
        // Iterate through each neighbour of current node
        for(unsigned int i = 0; i < stations[currentnode].size(); i++){
            // Push neighbour to heap if not already visited
            if (std::find(visited.begin(), visited.end(), stations[currentnode].at(i)->getDestination()) == visited.end()) {
                heap.push(HeapNode(stations[currentnode].at(i),
                                   mintime+stations[currentnode].at(i)->getTraveltime(), lastconnection));
            }
        }
        do {
            if(heap.empty()){
                // Heap empty, problem occured
                mintime = -1;
                std::cout << "No path could be found! :(" << std::endl;
                return;
            }
            // Pop node with least weight from heap and update currentnode and mintime
            HeapNode node = heap.top();
            currentnode = node.connection->getDestination();
            mintime = node.weight;
            lastconnection = node.connection;
            heap.pop();
            lastcons.insert({currentnode, node.lastconnection});
            // Ignore nodes which are already visited, go to next one
        }while (std::find(visited.begin(), visited.end(), currentnode) != visited.end());
        // Do as long as current node is not start station
    } while (currentnode!=start_station);

    // Print found path
    printPath(lastcons, start_station, end_station);
}

const void ConnectionPlan::printPath(std::map<std::string,Connection*> path, std::string start_station,
                                     std::string end_station) {
    int time = 0;
    Connection* con = path[start_station];
    std::string laststation = "";
    std::string metroline = con->getLine();

    std::cout << "Using " << metroline << std::endl << std::endl;

    std::cout << "Starting from " << start_station << std::endl;
    do{
        time += con->getTraveltime();
        std::cout << "in " << con->getTraveltime() << " Minutes to " << con->getDestination() << std::endl;

        if(con->getLine().compare(metroline) != 0){
            metroline = con->getLine();
            std::cout << std::endl << "In " << con->getDestination() << " change to " << metroline
                      << std::endl << std::endl;
        }

        laststation = con->getDestination();
        con = path[con->getDestination()];
    }while(con != nullptr);

    for(unsigned int i = 0; i < stations[laststation].size(); i++){
        if(stations[laststation].at(i)->getDestination() == end_station){
            std::cout << "in " << stations[laststation].at(i)->getTraveltime() << " Minutes to " <<
                      stations[laststation].at(i)->getDestination() << std::endl;
            time += stations[laststation].at(i)->getTraveltime();
        }
    }

    std::cout << std::endl << "Total time needed: " << time << " Minutes" << std::endl;
}

const void ConnectionPlan::printAdjacencyList(){
    for (auto it = stations.begin(); it != stations.end(); ++it ){
        std::cout << "Station: " << it->first << std::endl;
        for(unsigned int i = 0; i < it->second.size(); i++){
            std::cout << it->second.at(i) << std::endl;
            delete it->second.at(i);
            it->second.erase(it->second.begin()+i);
        }
    }
}