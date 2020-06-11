/*#include "dataparser.h"
#include "reporter.h"

void printClusterGraph(Cluster * cluster) {
        if (cluster == nullptr) {
            return;
        }
        std::cout << "Cluster range: ";
        for(int i = 0; i < cluster->GetRanges().size(); i++) {
            std::cerr << cluster->GetRanges()[i] << std::endl;
        } 
        std::cerr << std::endl;
        std::cerr << "Cluster count: " << cluster->GetCount() << std::endl;
        for (int i = 0; i < cluster->GetRanges().size(); i++) {
            std::cerr << "Left Child Dim " << i << ": "<< std::endl;
            printClusterGraph(cluster->GetChild(i, LEFT));
            std::cerr << "Right Child Dim " << i << ": "<< std::endl;
            printClusterGraph(cluster->GetChild(i, RIGHT));
        }
        
}
*/

int main(int argc, char* argv[]) 
{
  return 0;
  /*
  std::vector<std::vector<std::string>> testData = {
        {"col1", "col2"},
        {   "-1",    "-1"},
        {   "-1",    "1"},
        {   "5",    "-1"},
        {   "5",    "1"},
        {   "15",    "-1"},
        {   "15",    "1"},
        {   "20",    "-1"},
        {   "20",    "1"}};

    ClusterGraph *cluster_graph;
    Config c;
    
    Parse("test5.conf", &c);
    cluster_graph = new ClusterGraph(&c);
    unsigned int readCount, trueOffset = 1, batchSize = 10;
    DataParser dp(c, batchSize);

    dp.LoadHeaders();
    while (readCount = dp.LoadNextDataBatch(cluster_graph))
    {
    }
    printClusterGraph(cluster_graph->GetRoot());
    */
}