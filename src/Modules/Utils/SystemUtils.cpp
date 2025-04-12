#include "../include/Modules/Utils/SystemUtils.hpp"

std::string SystemUtils::getEnvVar(const std::string& name)
{
    char buffer[256];
    DWORD length = GetEnvironmentVariableA(name.c_str(), buffer, sizeof(buffer));

    if (length == 0 || length >= sizeof(buffer))
    {
        throw std::runtime_error("Failed to get environment variable: " + name);
    }
    return std::string(buffer);
}
