#include <pch.hpp>

#include <Modules/Utils/SystemUtils.hpp>
#include <Modules/Data/DataPipelineManager.hpp>
#include <Modules/Data/DataFetcher.hpp>
#include <Modules/Data/DatabaseHandler.hpp>
#include <Modules/PyIntegration/PythonBridge.hpp>

void run_eda()
{       
    try
    {
       /* py::module sys = py::module::import("sys");
        sys.attr("path").attr("insert")(0, PYTHON_DIR);
        py::module runner = py::module::import("runner");
        runner.attr("run")(pythonTaskToString(PythonTasks::EDA));*/

        // SCRIPTING FROM c++ 
    //    Py_Initialize();
    //    py::module sys = py::module::import("sys");
    //    py::module runner = py::module::import("runner");
    //    runner.attr("run")("eda");

    //    py::exec(R"(
    //    import pandas as pd
    //    import matplotlib.pyplot as plt
    //    import psycopg2
    //    from matplotlib.dates import DateFormatter
    //    
    //    # DB config
    //    DB_NAME = "backtester_local"
    //    DB_USER = "postgres"
    //    DB_PASS = "1111"
    //    DB_HOST = "localhost"
    //    DB_PORT = "5432"
    //    
    //    # Connect and query
    //    conn = psycopg2.connect(
    //        dbname=DB_NAME, user=DB_USER, password=DB_PASS, host=DB_HOST, port=DB_PORT
    //    )
    //    
    //    query = """
    //        SELECT time, open, high, low, close, volume
    //        FROM ohlcv_data
    //        ORDER BY time ASC
    //        LIMIT 1000;
    //    """
    //    
    //    df = pd.read_sql(query, conn, parse_dates=['time'])
    //    conn.close()
    //    
    //    # Plot
    //    plt.figure(figsize=(12, 6))
    //    plt.plot(df['time'], df['close'], label='Close Price')
    //    plt.title("OHLCV Close Prices")
    //    plt.xlabel("Time")
    //    plt.ylabel("Price")
    //    plt.grid(True)
    //    plt.legend()
    //    plt.gca().xaxis.set_major_formatter(DateFormatter('%Y-%m-%d %H:%M'))
    //    plt.xticks(rotation=45)
    //    plt.tight_layout()
    //    plt.show()
    //)");
    }
    catch (const py::error_already_set& e)
    {
        std::cerr << "Python Error: " << e.what() << std::endl;
    }
}

int main(int argc, char** argv)
{
    std::cout << "HelloCpp!\n";

#ifdef _WIN32
    // python init. 
    SystemUtils::setEnvVar(PYTHONPATH, PYTHON_DIR);
    // DB init.  
    std::string dbName = SystemUtils::getEnvVar(DB_NAME);
    std::string dbUser = SystemUtils::getEnvVar(DB_USER);
    std::string dbPass = SystemUtils::getEnvVar(DB_PASS);
    std::string dbHost = SystemUtils::getEnvVar(DB_HOST);
#else
    setenv("PYTHONPATH", "/in/future/linux/env/Backtester/scripts", 1);
#endif

    // Data pipeline - fetch, clean(?), transform(?), upload to DB.
    // for now, simply fetch and upload to DB using timescale.
    DatapipelineManager dataPipe;
    dataPipe.addStage(std::make_unique<DataFetcher>());
    dataPipe.addStage(std::make_unique<DatabaseHandler>(dbName, dbUser, dbPass, dbHost));
    nlohmann::json data = dataPipe.run();

    // Python integration checks 
    PythonBridge bridge{};
    bridge.CallFunction(pythonTaskToString(PythonTasks::EDA));
    return EXIT_SUCCESS;
}