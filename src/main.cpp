#include <pch.hpp>

#include <Modules/Utils/SystemUtils.hpp>
#include <Modules/Data/DataPipelineManager.hpp>
#include <Modules/Data/DataFetcher.hpp>
#include <Modules/Data/DatabaseHandler.hpp>
#include <Modules/PyIntegration/PythonBridge.hpp>

int main(int argc, char** argv)
{
    std::cout << "HelloCpp!\n";

#ifdef _WIN32
    // python init. 
    SystemUtils::setEnvVar(PYTHONPATH, PYTHON_DIR);
    // DB init.  
    std::string dbName = SystemUtils::getEnvVar(DB_NAME);
    std::string dbUser = SystemUtils::getEnvVar(DB_USER);
    std::string dbPass = SystemUtils::getEnvVar(DB_PASS);
    std::string dbHost = SystemUtils::getEnvVar(DB_HOST);
#else
    setenv("PYTHONPATH", "/in/future/linux/env/Backtester/scripts", 1);
#endif

    // Data pipeline - fetch, clean(?), transform(?), upload to DB.
    // for now, simply fetch and upload to DB using timescale.
    DatapipelineManager dataPipe;
    dataPipe.addStage(std::make_unique<DataFetcher>());
    dataPipe.addStage(std::make_unique<DatabaseHandler>(dbName, dbUser, dbPass, dbHost));
    nlohmann::json data = dataPipe.run();

    // Python integration checks 
    PythonBridge bridge{};
    bridge.CallFunction(pythonTaskToString(PythonTasks::EDA));
    return EXIT_SUCCESS;
}