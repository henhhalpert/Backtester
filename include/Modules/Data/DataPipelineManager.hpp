#pragma once

#include "IDataStage.hpp"

class DatapipelineManager
{
public:
	DatapipelineManager() = default;
	void addStage(std::unique_ptr<IDataStage> stage);
	void run();
private:
	std::vector<std::unique_ptr<IDataStage>> stages;
};
