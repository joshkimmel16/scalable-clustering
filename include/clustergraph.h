// Define clusters and graph that contains them

#ifndef CLUSTERGRAPH_H
#define CLUSTERGRAPH_H

#include <string>
#include <vector>

#include "datapoint.h"
#include "compare.h"

class Cluster {
public:
    Cluster(DataPoint* anch); //create a cluster with an anchor
    ~Cluster(); //destructor
    void AddParent(Cluster* p); //add a parent for the given cluster
    void AddChild(Cluster* p); //add a child for the given cluster
    std::vector<Cluster*> ProcessDataPoint(DataPoint* p, Config* conf); //handle incoming data point
private:
    unsigned int count; //track # data points matching cluster
    std::vector<Cluster*> parents; //track parents of cluster
    std::vector<Cluster*> Children; //track children of cluster
    DataPoint* anchor; //DataPoint to use to compare to incoming points
};

class ClusterGraph {
public:
    ClusterGraph(DataPoint* r); //constructor (need root data point)
    ~ClusterGraph(); //destructor
    void AddCluster(Cluster* parent, Cluster* new_cluster); //add a cluster to the given parent cluster in the graph
private:
    Cluster* root; //root of the graph
};

#endif