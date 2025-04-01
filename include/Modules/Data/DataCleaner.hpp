#pragma once
#include "IDataStage.hpp"

class DataCleaner : public IDataStage
{
public:
	DataCleaner() = default;
	virtual void process(nlohmann::json& data);
};
