#include <pch.hpp>

#include <Modules/Data/DataPipelineManager.hpp>
#include <Modules/Data/DataFetcher.hpp>
#include <Modules/Data/DatabaseHandler.hpp>


namespace py = pybind11;

void run_eda() {
    py::scoped_interpreter guard{}; // start Python interpreter

    try {
        py::module_ sys = py::module_::import("sys");
        sys.attr("path").attr("append")("scripts/eda"); // or absolute path

        py::module_ eda = py::module_::import("ohlcv_eda"); // corresponds to ohlcv_eda.py
        // If you want to call a function inside the script:
        // py::object result = eda.attr("your_function_name")();
    }
    catch (const py::error_already_set& e) {
        std::cerr << "Python Error: " << e.what() << std::endl;
    }
}

int main(int argc, char** argv)
{
    std::string dbName      = std::string(std::getenv("DB_NAME")     ? std::getenv("DB_NAME")     : "");
    std::string dbUser      = std::string(std::getenv("DB_USER")     ? std::getenv("DB_USER")     : "");
    std::string dbPass      = std::string(std::getenv("DB_PASSWORD") ? std::getenv("DB_PASSWORD") : "");
    std::string dbHost      = std::string(std::getenv("DB_HOST")     ? std::getenv("DB_HOST")     : "");

    // Data pipeline - fetch, clean(?), transform(?), upload to DB.
    // for now, simply fetch and upload to DB using timescale.
    DatapipelineManager dataPipe;
    dataPipe.addStage(std::make_unique<DataFetcher>());
    dataPipe.addStage(std::make_unique<DatabaseHandler>(dbName, dbUser, dbPass, dbHost));
    nlohmann::json data = dataPipe.run();
    

    return EXIT_SUCCESS;
}