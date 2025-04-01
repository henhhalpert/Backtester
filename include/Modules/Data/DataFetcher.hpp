#pragma once

#include "IDataStage.hpp"

class DataFetcher : public IDataStage
{
public:
    DataFetcher() = default;
    void process(nlohmann::json& data) override;
    void processAsync();
private:
    
};