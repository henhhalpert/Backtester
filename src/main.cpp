#include <pch.hpp>

#include <Modules/Data/DataPipelineManager.hpp>
#include <Modules/Data/DataFetcher.hpp>

int main(int argc, char** argv)
{
    // testing data fetcher - all tests will move to tests folder, later.
    //1, add fetcher and see if it fetches a single packet of data 
    DatapipelineManager dataPipe;
    dataPipe.addStage(std::make_unique<DataFetcher>());
    dataPipe.run();


    return EXIT_SUCCESS;
}