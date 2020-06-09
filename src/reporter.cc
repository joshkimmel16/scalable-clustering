#include "reporter.h"

std::vector<Cluster *> * Reporter::CompressAndGenerateReport() {
    CompressClusterGraph();
    GenerateReport();
    return &reportedClusters;
}

void Reporter::CompressClusterGraph() {
    if(graph != nullptr) {
        CompressClusterGraph(graph->GetRoot());
    }
}

bool Reporter::CompressClusterGraph(Cluster * cluster) {
    if (cluster == nullptr) {
        return true;
    }
    else if (cluster->GetCount() > threshold) {
        for(int i=0; i < cluster->GetChildren().size(); i++) {
            Cluster* leftChild = cluster->GetChild(i, LEFT);
            Cluster* rightChild = cluster->GetChild(i, RIGHT);

            if(leftChild != nullptr) {
                if (!CompressClusterGraph(leftChild)) {
                    delete leftChild;
                    cluster->GetChildren(i).SetLeft(nullptr);
                }
            }
            if (rightChild != nullptr) {
                if (!CompressClusterGraph(rightChild)) {
                    delete rightChild;
                    cluster->GetChildren(i).SetRight(nullptr);
                }
            }
        }
        return true;
    }
    else {
        return false;
    }
}

void Reporter::GenerateReport() {
    if(graph != nullptr) {
        CompressClusterGraph(graph->GetRoot());
    }
} 

void Reporter::GenerateReport(Cluster * cluster) {
    if (cluster == nullptr) {
        return;
    }

    bool isLeaf = true;
    for(int i=0; i < cluster->GetChildren().size(); i++) {
            Cluster* leftChild = cluster->GetChild(i, LEFT);
            Cluster* rightChild = cluster->GetChild(i, RIGHT);

            if(leftChild != nullptr || rightChild != nullptr) {
               isLeaf = false;
            }
    }
    if (isLeaf) {
        reportedClusters.push_back(cluster);
    }
    else {
        std::vector<unsigned int> sum(cluster->GetChildren().size(), 0);
        for(int i=0; i < cluster->GetChildren().size(); i++) {
            Cluster* leftChild = cluster->GetChild(i, LEFT);
            Cluster* rightChild = cluster->GetChild(i, RIGHT);

            if(leftChild != nullptr) {
                GenerateReport(leftChild);
                sum[i]+= leftChild->GetCount();
            }
            if(rightChild != nullptr) {
                GenerateReport(rightChild);
                sum[i] += rightChild->GetCount();
            }
        }
        unsigned int maxSum = *max_element(sum.begin(), sum.end());

        if (cluster->GetCount() - maxSum >= threshold) {
            reportedClusters.push_back(cluster);
        }
        else {
            cluster->SetCount(maxSum);
        }
    }
}