/* Copyright Alexander Dietrich und Selina Brinnich */

#ifndef JOURNEYPLANNER_ADJACENCYLIST_H
#define JOURNEYPLANNER_ADJACENCYLIST_H

#include <iostream>
#include <unordered_map>
#include <map>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>
#include <string.h>
#include <algorithm>

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
    const std::string getDestination();
    const std::string getLine();
    const int getTraveltime();
};

/*
 * Class for entries of priority list (heap for dijkstra algorithm)
 */
class HeapNode {
public:
    HeapNode(Connection* connection, int weight, Connection* lastconnection);
    Connection* connection;
    int weight;
    Connection* lastconnection;
};

/*
 * Class for comparing entries in priority list (heap for dijkstra algorithm)
 */
class CompareHeapNode {
public:
    bool operator()(HeapNode& n1, HeapNode& n2)
    {
        if (n1.weight > n2.weight) return true;
        return false;
    }
};

/*
 * Class implementing the adjacency list and search algorithm for shortest path
 */
class ConnectionPlan {
private:
    // Time for changing lines (in minutes)
    int changingtime = 5;

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

    /**
     * Print a specified path of connections
     * @param path a map holding nodes as keys and connections as values
     * @param start_station name of station to start from
     * @param end_station name of destination station
     */
    const void printPath(std::map<std::string,Connection*> path, std::string start_station, std::string end_station);

public:
    ConnectionPlan();
    ~ConnectionPlan();

    /**
     * Builds an adjacency list by reading a transport network from a file
     * @param filepath path to file containing the transport network
     */
    void readFromFile(std::string filepath);

    /**
     * A shortest path search algorithm (Dijkstra) to find the fastest way from given start to destination
     * Prints out the result to cout
     * @param start_station name of station to start from
     * @param end_station name of destination station
     */
    const void getShortestPath(std::string start_station, std::string end_station);

    /**
     * Prints the adjacency list (only for debugging purposes)
     */
    const void printAdjacencyList();

    /**
     * Sets the time needed for changing lines
     * @param newtime new time in minutes
     * @return true if time could be set or false otherwise
     */
    bool setChangingTime(int newtime);
};

#endif //JOURNEYPLANNER_ADJACENCYLIST_H
