#ifndef REPORTER_H
#define REPORTER_H

#include <string>
#include <vector>
#include <algorithm>

#include "clustergraph.h"


class Reporter {
public:
    Reporter(ClusterGraph * g, double thresh) {
        graph = g;
        threshold = thresh;
    }
    std::vector<Cluster *> * CompressAndGenerateReport();

private:
    ClusterGraph * graph;
    double threshold;
    std::vector<Cluster *> reportedClusters;
    void CompressClusterGraph();
    bool CompressClusterGraph(Cluster * cluster);
    void GenerateReport();
    void GenerateReport(Cluster* cluster);

};

#endif