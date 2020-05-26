#include "clustergraph.h"

Cluster::Cluster (DataPoint* anch) {
    count = 0;
    anchor = anch;
}

Cluster::~Cluster() {
    delete anchor;
}

void Cluster::AddParent (Cluster* p) {
    parents.push_back(p);
}

void Cluster::AddChild (Cluster* p) {
    children.push_back(p);
}

std::vector<Cluster*> Cluster::ProcessDataPoint(DataPoint* p, Config* conf) {
    std::vector<Cluster*> toVisit;
    for (unsigned int i=0; i<conf->numArgs; i++) {
        unsigned int check = Compare(anchor, p, i, conf->types[i]);
        //TODO: how to add the right children to the output?
    }
}

ClusterGraph::ClusterGraph(DataPoint* r) {
    root = new Cluster(r);
}

ClusterGraph::~ClusterGraph() {
    //TODO: recurse through graph deleting all clusters (use stack)
    //be careful about only adding non-empty pointers onto the stack (since a cluster can have multiple parents)
}

void ClusterGraph::AddCluster(Cluster* parent, Cluster* new_cluster) {
    parent->AddChild(new_cluster);
    new_cluster->AddParent(parent);
}