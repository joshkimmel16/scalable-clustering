#ifndef REPORTER_H
#define REPORTER_H

#include <string>
#include <vector>
#include <algorithm>

#include "clustergraph.h"


class Reporter {
public:
    Reporter(Config *c, ClusterGraph * g) {
        config = c;
        graph = g;
        threshold = c->GetThreshold();
    }

    //used to compress,generate, and write report
    void CompressAndGenerateReport();

    //only compresses cluster graph
    void CompressClusterGraph();

    //sets reportedClusters and returns vector of clusters to be in report
    //must be called after CompressClusterGraph()
    std::vector<Cluster *> * GenerateReport();

private:
    Config * config;
    ClusterGraph * graph;
    double threshold;
    std::vector<Cluster *> reportedClusters;
    bool CompressClusterGraph(Cluster * cluster);
    void GenerateReport(Cluster* cluster);
    void WriteReport();

};

#endif