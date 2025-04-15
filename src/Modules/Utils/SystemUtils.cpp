#include "../include/Modules/Utils/SystemUtils.hpp"

std::string SystemUtils::getEnvVar(const std::string& name)
{
    char buffer[SYS_BUFFER_SIZE];
    DWORD length = GetEnvironmentVariableA(name.c_str(), buffer, sizeof(buffer));

    if (length == 0 || length >= sizeof(buffer))
    {
        throw std::runtime_error("Failed to get environment variable: " + name);
    }
    return std::string(buffer);
}

void SystemUtils::setEnvVar(const std::string& name, const std::string& value)
{
    SetEnvironmentVariableA(name.c_str(), value.c_str());
}