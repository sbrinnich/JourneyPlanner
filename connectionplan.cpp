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
            // Delete connection pointers and remove from connections list
            delete it->second.at(i);
            it->second.erase(it->second.begin()+i);
        }
    }
    // Remove all stations from map
    stations.clear();
}

void ConnectionPlan::readFromFile(std::string filepath) {
    std::string line = "", metro="", read = "", laststation = "";
    int traveltime = 0;
    bool stationnext;
    // Open file
    std::ifstream file(filepath, std::ios::in);
    // Read until end of file
    while (!file.eof()) {
        // Get line
        getline(file, line, '\n');
        // If line not empty
        if(line.length() > 0){
            std::istringstream stream(line);
            // Read line name at the beginning of each line
            getline(stream, metro, ':');
            // Next comes a station, not a traveltime
            stationnext = true;
            traveltime = 0;
            laststation = "";
            // Read until end of line
            while(!stream.eof()){
                if(stationnext){
                    // Read station name between leading and trailing "
                    getline(stream, read, '"');
                    getline(stream, read, '"');
                    if(laststation.length() > 0){
                        // If not first station read, add a connection between this and last read station
                        addConnection(laststation, read, metro, traveltime);
                    }
                    laststation = read;
                }else{
                    // Read traveltime
                    getline(stream, read, ' ');
                    getline(stream, read, ' ');
                    // Parse string to int
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
                // Check if line changes
                if(lastconnection == nullptr || lastconnection->getLine() == stations[currentnode].at(i)->getLine()) {
                    heap.push(HeapNode(stations[currentnode].at(i),
                                       mintime + stations[currentnode].at(i)->getTraveltime(),
                                       lastconnection));
                }else{
                    // If line changes, add time for changing
                    heap.push(HeapNode(stations[currentnode].at(i),
                                       mintime + stations[currentnode].at(i)->getTraveltime() + changingtime,
                                       lastconnection));
                }
            }
        }
        do {
            if(heap.empty()){
                // Heap empty, problem occured
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
    // Init
    int totaltime = 0, nexttime = 0;
    Connection* con = path[start_station];
    std::string laststation = "", metroline;

    // Calculate time needed from first to second station
    for(unsigned int i = 0; i < stations[start_station].size(); i++){
        if((con != nullptr && stations[start_station].at(i)->getDestination() == con->getDestination()) ||
                // If con = nullptr, startstation directly connected to endstation
                stations[start_station].at(i)->getDestination() == end_station ){
            // Find shortest way from first to second station
            if(nexttime == 0 || nexttime > stations[start_station].at(i)->getTraveltime()) {
                // Set time for next station
                nexttime = stations[start_station].at(i)->getTraveltime();
                // Get line for using first
                metroline = stations[start_station].at(i)->getLine();
            }
        }
    }

    // Increase total time
    totaltime += nexttime;

    // Print line and starting station
    std::cout << "Using " << metroline << std::endl << std::endl;
    std::cout << "Starting from " << start_station << std::endl;

    // Print path
    while(con != nullptr){
        // Output needed time to next station
        std::cout << "in " << nexttime << " Minutes to " << con->getDestination() << std::endl;

        // Set time for next station
        nexttime = con->getTraveltime();

        // Increase total time
        totaltime += nexttime;

        // Output line change if needed
        if(con->getLine().compare(metroline) != 0){
            metroline = con->getLine();
            std::cout << std::endl << "In " << con->getDestination() << " change to " << metroline
                      << " (" << changingtime << " Minutes)" << std::endl << std::endl;
            totaltime += changingtime;
        }

        laststation = con->getDestination();
        // Set to next station
        con = path[con->getDestination()];
    }

    // Output last connection to destination
    std::cout << "in " << nexttime << " Minutes to " << end_station << std::endl;

    // Print total time needed
    std::cout << std::endl << "Total time needed: " << totaltime << " Minutes" << std::endl;
}

bool ConnectionPlan::setChangingTime(int newtime) {
    if(newtime >= 0){
        this->changingtime = newtime;
        return true;
    }
    return false;
}

// For testing purposes
const void ConnectionPlan::printAdjacencyList(){
    for (auto it = stations.begin(); it != stations.end(); ++it ){
        std::cout << "Station: " << it->first << std::endl;
        for(unsigned int i = 0; i < it->second.size(); i++){
            std::cout << it->second.at(i)->getDestination() << std::endl;
        }
    }
}