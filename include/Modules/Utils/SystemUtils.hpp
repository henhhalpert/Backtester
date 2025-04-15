#include <pch.hpp>

#pragma once

class SystemUtils
{
public:
	static void  setEnvVar(const std::string& name, const std::string& value);
	static std::string getEnvVar(const std::string& name);
private:

};