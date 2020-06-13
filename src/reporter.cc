#include "reporter.h"

void Reporter::CompressAndGenerateReport(std::vector<std::string> colNames) {
    CompressClusterGraph();
    GenerateReport();
    WriteReport(colNames);
}

void Reporter::CompressClusterGraph() {
    if(graph != nullptr) {
        CompressClusterGraph(graph->GetRoot());
    }
}

//perform DFS on cluster graph
//prune leaves less than threshold
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
                    //prune tree for any node less than threshold
                    delete leftChild;
                    cluster->GetChildren(i).SetLeft(nullptr);
                }
            }
            if (rightChild != nullptr) {
                if (!CompressClusterGraph(rightChild)) {
                    //prune tree for any node less than threshold
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
    //return if nullptr or already visited
    if (cluster == nullptr || cluster->GetFlag(0) == 1) {
        return;
    }

    //set flag so nodes aren't visited twice
    cluster->SetFlags(1);

    bool isLeaf = true;
    for(int i=0; i < cluster->GetDimension(); i++) {
            Cluster* leftChild = cluster->GetChild(i, LEFT);
            Cluster* rightChild = cluster->GetChild(i, RIGHT);

            if(leftChild != nullptr || rightChild != nullptr) {
               isLeaf = false;
            }
    }
    //all leaves are added to report
    if (isLeaf) {
        reportedClusters.push_back(cluster);
    }
    else {
        std::vector<unsigned int> sum(cluster->GetDimension(), 0);
        for(int i=0; i < cluster->GetDimension(); i++) {
            Cluster* leftChild = cluster->GetChild(i, LEFT);
            Cluster* rightChild = cluster->GetChild(i, RIGHT);

            if(leftChild != nullptr) {
                //DFS
                GenerateReport(leftChild);
                sum[i] += leftChild->GetCount();
            }
            if(rightChild != nullptr) {
                //DFS
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

void Reporter::WriteReport(std::vector<std::string> colNames) {
    std::ofstream reportFile;
    reportFile.open (config->GetReportPath());
    reportFile << "Report Containing Compressed Cluster List " <<std::endl;
    reportFile << "----------------------------------------- " <<std::endl;
    for (int i=0; i < reportedClusters.size(); i++) {
        reportFile << "Cluster: " << std::endl;
        reportFile << "Count: " << reportedClusters[i]->GetCount() << std::endl;
        reportFile << "Range in Each Dimension: " << std::endl;
        for (int j=0; j < colNames.size(); j++) {
            reportFile << std::fixed << std::left << 
            std::setw(
                std::max(colNames[j].length(), reportedClusters[i]->GetName(j).length())+3) << 
            std::setfill(' ') << colNames[j];
        }
        reportFile << std::endl;
        for (int j=0; j < colNames.size(); j++) {
            reportFile << std::fixed << std::left << 
            std::setw(
                std::max(colNames[j].length(), reportedClusters[i]->GetName(j).length())+3) << 
            std::setfill(' ') << reportedClusters[i]->GetName(j);        }
        reportFile << std::endl;
        reportFile << "----------------------------------------- " <<std::endl;
    }
    reportFile.close();
}