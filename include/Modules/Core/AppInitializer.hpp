#pragma once
#include <pch.hpp>

// AppInitializer.hpp
class AppInitializer
{
public:
    static void initialize();
    static void setupPython();
    static void setupDatabaseEnv(std::string& dbName, std::string& dbUser, std::string& dbPass, std::string& dbHost);
};