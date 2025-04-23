#include "../include/Modules/Core/AppRunner.hpp"

// Utils
#include <../include/Modules/Utils/SystemUtils.hpp>
// Data
#include <../include/Modules/Data/DataPipelineManager.hpp>
#include <../include/Modules/Data/DataFetcher.hpp>
#include <../include/Modules/Data/DatabaseHandler.hpp>
// Python
#include <../include/Modules/PyIntegration/PythonBridge.hpp>

void AppRunner::run()
{
    std::string dbName, dbUser, dbPass, dbHost;
    AppInitializer::setupDatabaseEnv(dbName, dbUser, dbPass, dbHost);

    DatapipelineManager dataPipe;
    dataPipe.addStage(std::make_unique<DataFetcher>());
    dataPipe.addStage(std::make_unique<DatabaseHandler>(dbName, dbUser, dbPass, dbHost));
    auto data = dataPipe.run();

    PythonBridge bridge;
    bridge.CallFunction(pythonTaskToString(PythonTasks::EDA));
}