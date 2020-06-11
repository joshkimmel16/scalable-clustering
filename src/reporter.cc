#include "reporter.h"

void Reporter::CompressAndGenerateReport() {
    CompressClusterGraph();
    GenerateReport();
    WriteReport();
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

    else if (cluster->GetCount() >= threshold) {
        //clear flags for GenerateReport
        cluster->SetFlags(0);

        for(int i=0; i < cluster->GetDimension(); i++) {
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

std::vector<Cluster *> * Reporter::GenerateReport() {
    if(graph != nullptr) {
        GenerateReport(graph->GetRoot());
    }
    return &reportedClusters;
} 

void Reporter::GenerateReport(Cluster * cluster) {
    if (cluster == nullptr || cluster->GetFlag(0) == 1) {
        return;
    }

    cluster->SetFlags(1);

    bool isLeaf = true;
    for(int i=0; i < cluster->GetDimension(); i++) {
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
        std::vector<unsigned int> sum(cluster->GetDimension(), 0);
        for(int i=0; i < cluster->GetDimension(); i++) {
            Cluster* leftChild = cluster->GetChild(i, LEFT);
            Cluster* rightChild = cluster->GetChild(i, RIGHT);

            if(leftChild != nullptr) {
                GenerateReport(leftChild);
                sum[i] += leftChild->GetCount();
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

void Reporter::WriteReport() {
    std::ofstream reportFile;
    reportFile.open (config->GetReportPath());
    reportFile << "Report Containing Compressed Cluster List " <<std::endl;
    reportFile << "----------------------------------------- " <<std::endl;
    for (int i=0; i < reportedClusters.size(); i++) {
        reportFile << "Cluster: " << std::endl;
        reportFile << "Count: " << reportedClusters[i]->GetCount() << std::endl;
        reportFile << "Range: ";
        for (int j=0; j < reportedClusters[i]->GetRanges().size(); j++)
                        reportFile << reportedClusters[i]->GetRanges()[j] << " ";
        reportFile << std::endl;
        reportFile << "----------------------------------------- " <<std::endl;
    }
    reportFile.close();
}