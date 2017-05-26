/* Copyright Alexander Dietrich und Selina Brinnich */

#ifndef JOURNEYPLANNER_ADJACENCYLIST_H
#define JOURNEYPLANNER_ADJACENCYLIST_H

#include <iostream>
#include <unordered_map>
#include <vector>

/*
 * Class holding the edges
 */
class Connection {
private:
    // Name of connected station
    std::string destination;
    // Name of metro line which connects the two stations
    std::string line;
    // Time to travel to connected station
    int traveltime;

public:
    Connection(std::string destination, std::string line, int traveltime);
};

/*
 * Class implementing the adjacency list and search algorithm for shortest path
 */
class ConnectionPlan {
private:
    // Adjacency list
    std::unordered_map<std::string, std::vector<Connection*> > stations;

    /**
     * Adds a connection into the connection plan
     * @param first_station Name of the first station
     * @param second_station Name of the second station
     * @param line Name of metro line which connects the two stations
     * @param traveltime Time in minutes to travel between the two stations
     */
    void addConnection(std::string first_station, std::string second_station, std::string line, int traveltime);

public:
    ConnectionPlan();
    ~ConnectionPlan();

    void readFromFile(std::string filepath);

    /**
     * A shortest path search algorithm (Dijkstra) to find the fastest way from given start to destination
     * @param start_station name of station to start from
     * @param end_station name of destination station
     * @return a vector holding the connections of the shortest path found
     */
    std::vector<Connection*> getShortestPath(std::string start_station, std::string end_station);

    /**
     * Print a specified path of connections
     * @param path a vector holding connections
     */
    void printPath(std::vector<Connection*> path);
};

#endif //JOURNEYPLANNER_ADJACENCYLIST_H
