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
            DataPoint *anch); // constructor
    ~Cluster(); //destructor
    void PopulateChildren(); // populate children in all dimensions
    void PopulateChildren(const unsigned int &d); // populate two children of children[d]
    void ProcessDataPoint(const int &id, DataPoint *p); // handle one incoming data point (one row from the csv file)

    void SetParent(const unsigned int &d, Cluster *p); // setter for parents[d]
    void SetChild(const unsigned int &d, BranchDirection direction,
                  Cluster *p); // setter for LEFT/RIGHT of children[d]
    void IncrementCount(); // count++
    void SetCount(unsigned int c);
    void SetFlag(const unsigned int &d, const int &flag); // setter for flags[d]
    void SetFlags(const int &flag); // set every flag to the value
    void ResetFlags(); // reset every flag to 0

    unsigned int GetDimension() const; // getter for dimension
    std::vector<Cluster *> GetParents() const; // getter for parents
    Cluster *GetParent(const unsigned int &d) const; // getter for parent[d]
    std::vector<Branch> GetChildren() const; // getter for children
    Branch GetChildren(const unsigned int &d) const; // getter for children[d]
    Cluster *GetChild(const unsigned int &d, BranchDirection direction) const; // getter for LEFT/RIGHT of children[d]
    DataPoint *GetAnchor() const; // getter for anchor
    const std::vector<unsigned int> &GetRanges() const; // getter for ranges
    unsigned int GetCount() const; // getter for count
    const int &GetFlag(const unsigned int &d) const; // getter for flags[d]
private:
    Config *config; // pointer to the config
    ClusterGraph *graph; // pointer to the graph
    const unsigned int dimension; // # of dimensions
    std::vector<Cluster *> parents; // track parents of cluster
    std::vector<Branch> children; // track children of cluster
    DataPoint *anchor; // DataPoint to use to compare to incoming points
    const std::vector<unsigned int> ranges; // list of ranges in cutoff values ([starting_index, ending_index]) e.g., [[0,10], [2,3], [1,3]] =flattened=> [0,10,2,3,1,3]
    unsigned int count; // track # of data points matching this cluster
    std::vector<int> flags; // flags
};

class Branch {
public:
    Branch() {
        left = nullptr;
        right = nullptr;
    }; // constructor
    ~Branch() {}; // destructor
    void SetLeft(Cluster *p); // setter for the left child
    void SetRight(Cluster *p); // setter for the right child
    Cluster *GetLeft() const; // getter for the left child
    Cluster *GetRight() const; // getter for the right child
private:
    Cluster *left;
    Cluster *right;
};

class ClusterGraph {
public:
    ClusterGraph(Config *conf); //constructor (need root data point)
    ~ClusterGraph(); //destructor
    std::vector<Cluster *> Search(std::vector<unsigned int> ranges); // search for a certain cluster.
    // if found, return its first parent (largest dimension #) and itself; if not found, return a vector of size 0
    void PopulateChildren(); // populate all children
    void ProcessDataPoint(const int &id, DataPoint *p); // handle one incoming data point (one row from the csv file)
    Cluster *GetRoot(); // getter for the root cluster
    unsigned int GetDimension() const; // getter for dimension
private:
    Cluster *root; //root of the graph
    const unsigned int dimension; // # of dimensions
};

#endif