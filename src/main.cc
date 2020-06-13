#include "clustergraph.h"
#include "config.h"
#include "reporter.h"
#include "dataparser.h"

int main(int argc, char* argv[]) 
{
    try {
        //need command line argument pointing to config file
        //assumed to be the first (and only) argument
        if (argc < 2) {
            std::cerr << "Missing required command line argument for config file!\n";
            return 1;
        }

        //read config file
        Config conf;
        Parse(argv[1], &conf);

        //generate cluster graph based on config
        ClusterGraph * cluster_graph = new ClusterGraph(&conf);
        cluster_graph->PopulateChildren();

        //initialize data parser
        unsigned int readCount, batchSize = conf.GetBatchSize();
        DataParser dp(conf, batchSize);
        dp.LoadHeaders();
        
        //read data into cluster graph in fixed batches
        while (readCount = dp.LoadNextDataBatch(cluster_graph)) {}

        //compress cluster graph and generate report
        Reporter * reporter = new Reporter(&conf, cluster_graph);
        reporter->CompressAndGenerateReport(dp.GetAttrNames());

        //memory cleanup
        delete reporter;
        delete cluster_graph;
    }
    catch (std::exception& e) {
        std::cerr << "Error occurred! " << e.what();
        return 1;
    }

    return 0;
}