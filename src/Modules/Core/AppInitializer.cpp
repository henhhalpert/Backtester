#include "../include/Modules/Core/AppInitializer.hpp"
#include "../include/Modules/Utils/SystemUtils.hpp"

void AppInitializer::initialize()
{
    setupPython();
}

void AppInitializer::setupPython()
{
#ifdef _WIN32
    SystemUtils::setEnvVar(PYTHONPATH, PYTHON_DIR);
#else
    setenv("PYTHONPATH", "/path/on/linux", 1);
#endif
}

void AppInitializer::setupDatabaseEnv(std::string& dbName, std::string& dbUser, std::string& dbPass, std::string& dbHost) {
    dbName = SystemUtils::getEnvVar(DB_NAME);
    dbUser = SystemUtils::getEnvVar(DB_USER);
    dbPass = SystemUtils::getEnvVar(DB_PASS);
    dbHost = SystemUtils::getEnvVar(DB_HOST);
}