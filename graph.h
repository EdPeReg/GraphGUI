#ifndef GRAPH_H
#define GRAPH_H

#include <algorithm>
#include <limits>
#include <set>
#include <vector>
#include <string>
#include <map>
#include <iostream>

class Vertex {
private:
    std::string name;
public:
    Vertex(const std::string &_name) : name(_name) { }

    std::string getName() const { return name; }
};

class Edge {
private:
    Vertex *origin;
    Vertex *dest;
    double distance;
public:
    Edge(Vertex *_origin, Vertex *_dest, double _distance) : origin(_origin),
                                                             dest(_dest),
                                                             distance(_distance) { }

    ~Edge() {
        // IS A GOOD IDEA?.
        delete origin;
        delete dest;
    }

    Vertex* getOrigin() const { return origin; }
    Vertex* getDest() const { return dest; }
    double getDistance() const { return distance; }
};

class Graph
{
private:
    std::map<Vertex *, std::vector<Edge *> > adjacencyList;

public:
    Graph() { }

    void insertEdge(Vertex *origin, Vertex *dest, double weight = 0) {
        Edge *edge = new Edge(origin, dest, weight);
        adjacencyList[origin].push_back(edge);
    }

    void toString() const {
        std::map<Vertex *, std::vector<Edge *> >::const_iterator mapIt;
        std::vector<Edge *>::const_iterator edgeIt;

        for(mapIt = adjacencyList.begin(); mapIt != adjacencyList.end(); mapIt++) {
            std::cout << "[" << mapIt->first << "] --> ";
            for(edgeIt = mapIt->second.begin(); edgeIt != mapIt->second.end(); edgeIt++) {
                Edge *edge = *edgeIt;
                std::cout << edge->getDest()->getName() << " ("
                          << std::to_string(edge->getDistance()) << "), ";
            }
        }
    }

    virtual ~Graph() {
        std::map<Vertex *, std::vector<Edge *> >::const_iterator mapIt;
        std::vector<Edge *>::const_iterator edgeIt;

        for(mapIt = adjacencyList.begin(); mapIt != adjacencyList.end(); mapIt++) {
            for(edgeIt = mapIt->second.begin(); edgeIt != mapIt->second.end(); edgeIt++) {
                Edge *edge = *edgeIt;
                delete edge->getDest();
                delete edge->getOrigin();
                delete edge;
            }
        }
    }
};

#endif
