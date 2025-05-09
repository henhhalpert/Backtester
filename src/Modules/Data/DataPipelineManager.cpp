#include "../include/Modules/Data/DataPipelineManager.hpp"

void DatapipelineManager::addStage(std::unique_ptr<IDataStage> stage)
{
	stages.emplace_back(std::move(stage));
}

nlohmann::json DatapipelineManager::run()
{
	nlohmann::json data;

	for (auto& stage : stages)
	{
		stage->process(data);
	}

	return data;
}
