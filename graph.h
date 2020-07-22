#ifndef GRAPH_H
#define GRAPH_H

/* The presented graph can be directed-undirected, with weight/without weight.
 * Dijstra algorithm implemented only working for graphs with weight. */

#include <QDebug>
#include <algorithm>
#include <limits>
#include <set>
#include <vector>
#include <string>
#include <map>
#include <iostream>

#include "particle.h"

struct Wrap {
    int origX, origY;
    int destX, destY;
};

class Node {
    private:
        Particle *particle;
        Wrap *wrap;
    public:
        Node(Particle *p) : particle(p) { }
        Node(Wrap *w) : wrap(w) { }

        Particle* getParticle() const { return particle; }
        Wrap* getWrap() const { return wrap; }
};

class Edge {
    private:
        Node *target;
        Wrap *wrap;
        double weight;
    public:
        Edge(Node *target, double weight = 0) {
            this->target = target;
            this->weight = weight;
        }

        Edge(Wrap *target, double weight = 0) {
            this->wrap = target;
            this->weight = weight;
        }

        Node* getTarget() const { return target; }
        Wrap* getWrap() const { return wrap; }
        double getWeight() const { return weight; }
};

class Graph {
    private:
        bool flag = true;
        bool flag2 = true;
        bool weighted, directed;
        int totalPathWeight;
        std::map<Wrap *, std::vector<Edge *>> adjacencyList;
    public:
        Graph() { }

        /* Fill find the node with the min distance. */
        struct distanceFinder {
            std::map<std::string, int> distanceTable;
            distanceFinder(std::map<std::string, int> d) : distanceTable(d) {  }

            bool operator()(const std::string &str1, const std::string &str2) {
                return distanceTable[str1] < distanceTable[str2];
            }
        };

        void setWeighted(bool w) { weighted = w; }
        void setDirected(bool d) { directed = d; }
        void setTotalPathWeight(int totalPathWeight) { this->totalPathWeight = totalPathWeight; }
        int getTotalPathWeight() const { return totalPathWeight; }
        bool getWeighted() const { return weighted; }
        bool getDirected() const { return directed; }

        void insertEdge(Particle *source, Particle *target, double weight = 0) {
//            qDebug() << "\ndentro de insertEdge";
//            qDebug() << "source: " << source->getId();
//            qDebug() << "target: " << target->getId();

            Wrap *wSource = new Wrap;
            Wrap *wTarget = new Wrap;
            wSource->origX = source->getOrigX();
            wSource->origY = source->getOrigY();
            wSource->destX = source->getDestX();
            wSource->destY = source->getDestY();

            wTarget->origX = target->getOrigX();
            wTarget->origY = target->getOrigY();
            wTarget->destX = target->getDestX();
            wTarget->destY = target->getDestY();

            Node* t = new Node(wTarget);
            Node* s = new Node(wSource);
            Edge *edgeSource = new Edge(s, weight);
            Edge *edgeTarget = new Edge(t, weight);

            adjacencyList[wSource].push_back(edgeTarget);

            // Because also our target node has a relation with out source node.
            adjacencyList[wTarget].push_back(edgeSource);

            std::map< Particle*, std::vector<Edge *> >::iterator it;
            std::vector<Edge *>::iterator eit;

//            for(it = adjacencyList.begin(); it != adjacencyList.end(); it++) {
//                qDebug() << "origen: " << it->first->getOrigX() << " " << it->first->getOrigY();
//                for(eit = it->second.begin(); eit != it->second.end(); eit++) {
//                    qDebug() << "destino: " << (*eit)->getTarget()->getParticle()->getDestX()
//                             << " " << (*eit)->getTarget()->getParticle()->getDestY();
//                }
//            }
        }

        std::vector<std::string> dijkstra(const Node *source, const Node *destination) {
            /*    std::map<Node *, int> dist;
            std::map<Node *, std::string> prev;
            std::set<Node *> nodes;

            std::map<std::string, std::vector<Edge *>>::iterator git;

            // For each vertex in our graph.
            for(git = adjacencyList.begin(); git != adjacencyList.end(); git++) {
                    // Add our vertex with an "infinite" value to our dist map.
                    dist[git->first] = std::numeric_limits<int>::max();

                    // Initialize with an undefined value.
                    prev[git->first] = "-1";

                    // Set all nodes in the graph.
                    nodes.insert(git->first);
            }

            // Distance from source to source.
            dist[source] = 0;

            // While our set of nodes aren't empty.
            while(!nodes.empty()) {
                    std::string minDisVertex = *std::min_element(nodes.begin(), nodes.end(), distanceFinder(dist));

                    // All remaining vertices are inaccessible from source.
                    if(dist[minDisVertex] == std::numeric_limits<int>::max()) {
                            break;
                    }

                    if(minDisVertex == destination) {
                            break;
                    }

                    nodes.erase(minDisVertex);

                    // Get all the edges that are adjacent to our node.
                    std::vector<Edge *> edges = adjacencyList.find(minDisVertex)->second;
                    for(std::vector<Edge *>::iterator it = edges.begin(); it != edges.end(); it++) {
                            Edge *edge = *it;

                            // if our vertex is in our nodes map.
                            if(nodes.find(edge->getTarget()->getName()) == nodes.end()) {
                                    continue;
                            }

                            int totalDistance = dist[minDisVertex] + edge->getWeight();

                            if(totalDistance < dist[edge->getTarget()->getName()]) {
                                    dist[edge->getTarget()->getName()] = totalDistance;
                                    prev[edge->getTarget()->getName()] = minDisVertex;
                            }
                    }
            }

            std::vector<std::string> path;
            if(getWeighted()) {
                    for(std::string n = destination; n != source;) {
                            path.push_back(n);
                            n = prev[n];
                    }
                    path.push_back(source);
                    setTotalPathWeight(dist[destination]);
            }

            return path*/;
        }

        void toString()  {
            std::map< Wrap *, std::vector<Edge *> >::const_iterator it;
            std::vector<Edge *>::const_iterator eit;
            for(it = adjacencyList.begin(); it != adjacencyList.end(); it++) {
                // it->first is our node source.

                // CASI PERO NO.
                if(flag) {
                    qDebug() << "[(" << it->first->origX << "," <<
                                        it->first->origY << ")] --> ";
                    flag = false;
                } else {
                    qDebug() << "[(" << it->first->destX << "," <<
                                        it->first->destY << ")] --> ";
                    flag = true;
                }
                for(eit = it->second.begin(); eit != it->second.end(); eit++) {
                    Edge *edge = *eit;

                    if(flag2) {
                        qDebug() << "(" << edge->getTarget()->getWrap()->destX << ","
                                         << edge->getTarget()->getWrap()->destY << ")" << " ("
                                         << (edge->getWeight()) << "), \n";
                        flag2 = false;
                    } else {
                        qDebug() << "(" << edge->getTarget()->getWrap()->origX << ","
                                         << edge->getTarget()->getWrap()->origY << ")" << " ("
                                         << (edge->getWeight()) << "), \n";
                        flag2 = true;
                    }
                }

                // LA IDEA ES QUE AHORA CUANDO VUELVA A REPETIR, ORIGEN SEA AHORA DESTINO.
            }
        }

        virtual ~Graph() {
        //    std::map< std::string, std::vector<Edge *> >::iterator it;
        //    std::vector<Edge *>::iterator eit;
        //    for(it = adjacencyList.begin(); it != adjacencyList.end(); it++) {
        //        for(eit = it->second.begin(); eit != it->second.end(); eit++) {
        //            Edge *edge = *eit;
        //            delete edge->getTarget();
        //            delete edge;
        //        }
        //    }
                        }
};

#endif
