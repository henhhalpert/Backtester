#pragma once

#include <pch.hpp>

// base class for all data pipeline steps
class IDataStage
{
public:
	virtual void process(nlohmann::json& data) = 0;
	virtual ~IDataStage() = default;
};
