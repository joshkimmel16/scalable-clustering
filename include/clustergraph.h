// Define clusters and graph that contains them

#ifndef CLUSTERGRAPH_H
#define CLUSTERGRAPH_H

#include <string>
#include <vector>

#include "datapoint.h"
#include "compare.h"
#include "config.h"

enum BranchDirection {
    LEFT,
    RIGHT
};

class Branch;

class ClusterGraph;

class Cluster {
public:
    Cluster(unsigned int d,
            std::vector<unsigned int> r,
            ClusterGraph *g,
            Config *conf,
            DataPoint *anch); //create a cluster with an anchor
    ~Cluster(); //destructor

    void PopulateChildren(); // populate children in all dimensions
    void PopulateChildren(unsigned int d); //populate two children (left and right) in dimension d
    void SetParent(unsigned int d, Cluster *p); // set/add a parent in dimension d for the given cluster
    void SetChild(unsigned int d, BranchDirection direction,
                  Cluster *p); // set/add a child in dimension d with direction (left/right) for the given cluster
    void SetAnchor(DataPoint *anchor); // setter for anchor
    const unsigned int GetDimension() const; // getter for dimension
    Cluster *GetParent(unsigned int d) const; // getter for parent[i]
    std::vector<Cluster *> GetParents() const; // getter for parents
    Branch GetBranch(unsigned int d) const; // getter for children
    std::vector<Branch> GetChildren() const; // getter for children
    DataPoint *GetAnchor() const; // getter for anchor
    const std::vector<unsigned int> &GetRanges() const; // getter for ranges

private:
    unsigned int count; //track # data points matching cluster
    const unsigned int dimension; // # of dimensions
    std::vector<Cluster *> parents; //track parents of cluster
    std::vector<Branch> children; //track children of cluster
    DataPoint *anchor; //DataPoint to use to compare to incoming points
    const std::vector<unsigned int> ranges; // list of ranges in cutoff values ([starting_index, ending_index]) e.g., [[0,10], [2,3], [1,3]] =flattened=> [0,10,2,3,1,3]
    Config *config; // pointer to the config
    ClusterGraph *graph; // pointer to the graph
};

class Branch {
public:
    Branch() {
        left = nullptr;
        right = nullptr;
    }; //constructor
    ~Branch() {}; //destructor
    void SetLeft(Cluster *p); // setter for left child
    void SetRight(Cluster *p); // setter for right child
    Cluster *GetLeft(); // retrieve left child
    Cluster *GetRight(); // retrieve right child
private:
    Cluster *left;
    Cluster *right;
};

class ClusterGraph {
public:
    ClusterGraph(Config *conf); //constructor (need root data point)
    ~ClusterGraph(); //destructor
    std::vector<Cluster *>
    Search(std::vector<unsigned int> ranges); // search for a certain cluster. if it has been added to the graph, return its parent (with the largest dimension number) and itself
    void PopulateChildren(); // populate children in all dimensions
    Cluster *GetRoot(); // getter for root
private:
    Cluster *root; //root of the graph
    const unsigned int dimension; // # of dimensions
};


#endif