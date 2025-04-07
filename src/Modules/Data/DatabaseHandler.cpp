#include "../include/Modules/Data/DatabaseHandler.hpp"

static const char* db_name      = std::getenv("DB_NAME");
static const char* db_user      = std::getenv("DB_USER");
static const char* db_password  = std::getenv("DB_PASSWORD");
static const char* db_host      = std::getenv("DB_HOST");

DatabaseHandler::DatabaseHandler(const std::string& db_name, const std::string& user, const std::string& password, const std::string& host)
    : conn_("dbname=" + db_name + " user=" + user + " password=" + password + " host=" + host)
{ 
    
    if (!conn_.is_open())
    {
        throw std::runtime_error("Failed to open connection to the database");
    }
}

// Destructor: Closes the database connection.
DatabaseHandler::~DatabaseHandler()
{
    if (conn_.is_open())
    {
        conn_.close();
    }
}

void DatabaseHandler::create_hyper_table_ohlcv(pqxx::work& txn) {
    txn.exec(R"(
        CREATE TABLE IF NOT EXISTS ohlcv_data (
            time TIMESTAMPTZ NOT NULL,
            symbol TEXT NOT NULL,
            open DOUBLE PRECISION,
            high DOUBLE PRECISION,
            low DOUBLE PRECISION,
            close DOUBLE PRECISION,
            volume DOUBLE PRECISION
        );
    )");

    // Ensure TimescaleDB extension is loaded
    txn.exec(R"(CREATE EXTENSION IF NOT EXISTS timescaledb CASCADE;)");

    // Create the hypertable (use explicit type casting if needed)
    txn.exec(R"(
        SELECT create_hypertable('ohlcv_data'::text, 'time'::text, if_not_exists => TRUE);
    )");
}

void DatabaseHandler::insert_data(pqxx::work& txn, const nlohmann::json& time_series, const std::string& symbol)
{
    for (auto it = time_series.begin(); it != time_series.end(); ++it)
    {
        const std::string& timestamp = it.key();  // e.g., "2025-04-03 11:00:00"
        const auto& values = it.value();

        double open = std::stod(values["1. open"].get<std::string>());
        double high = std::stod(values["2. high"].get<std::string>());
        double low = std::stod(values["3. low"].get<std::string>());
        double close = std::stod(values["4. close"].get<std::string>());
        double volume = std::stod(values["5. volume"].get<std::string>());

        txn.exec_params(
            R"(
                INSERT INTO ohlcv_data (time, symbol, open, high, low, close, volume)
                VALUES ($1, $2, $3, $4, $5, $6, $7)
            )",
            timestamp, symbol, open, high, low, close, volume
        );
    }
}

void DatabaseHandler::process(nlohmann::json& json_data)
{
    try
    {
        pqxx::work txn(conn_);

        // create hypertable using timescaleDB 
        create_hyper_table_ohlcv(txn);

        const auto& metadata = json_data["Meta Data"];
        const std::string symbol = metadata["2. Symbol"];

        const auto& time_series = json_data["Time Series (15min)"];

        // Insert OHLCV data into the database
        insert_data(txn, time_series, symbol);

        txn.commit();
        std::cout << "Inserted " << time_series.size() << " OHLCV records for " << symbol << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "DB error (inserting data): " << e.what() << std::endl;
    }
}

std::vector<std::tuple<std::string, double, double, double, double>>
DatabaseHandler::fetch_ohlcv_for_plot(const std::string& symbol, const std::string& start_time, const std::string& end_time)
{
    std::vector<std::tuple<std::string, double, double, double, double>> result;

    try
    {
        pqxx::work txn(conn_);
        pqxx::result r = txn.exec_params(
            R"(
                SELECT time, open, high, low, close
                FROM ohlcv_data
                WHERE symbol = $1
                AND time BETWEEN $2 AND $3
                ORDER BY time ASC
            )",
            symbol, start_time, end_time
        );

        for (const auto& row : r)
        {
            result.emplace_back(
                row["time"].c_str(),
                row["open"].as<double>(),
                row["high"].as<double>(),
                row["low"].as<double>(),
                row["close"].as<double>()
            );
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "DB error (retrieving data): " << e.what() << std::endl;
    }

    return result;
}

