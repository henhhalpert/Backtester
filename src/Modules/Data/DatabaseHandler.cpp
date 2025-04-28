#include "../include/Modules/Data/DatabaseHandler.hpp"

DatabaseHandler::DatabaseHandler(const std::string& db_name, const std::string& user, const std::string& password, const std::string& host)
    : m_dbName{ db_name }, m_user{ user }, m_pass{ password }, m_host{host}
{ 
}

// Destructor: Closes the database connection.
DatabaseHandler::~DatabaseHandler()
{
    
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
        const std::string timestamp = it.key() + " 00:00:00";  // e.g., "2025-04-03 11:00:00"
        const auto& values = it.value();

        double open = std::stod(values["1. open"].get<std::string>());
        double high = std::stod(values["2. high"].get<std::string>());
        double low = std::stod(values["3. low"].get<std::string>());
        double close = std::stod(values["4. close"].get<std::string>());
        double volume = std::stod(values["5. volume"].get<std::string>());

        txn.exec(
            pqxx::zview
            {
                R"(
                INSERT INTO ohlcv_data (time, symbol, open, high, low, close, volume)
                VALUES ($1, $2, $3, $4, $5, $6, $7)
                )"
            },
            pqxx::params{ timestamp, symbol, open, high, low, close, volume }
        );
    }
}

void DatabaseHandler::process(nlohmann::json& json_data)
{
    try
    {
        pqxx::connection conn{ "dbname=" + m_dbName + " user=" + m_user + " password=" + m_pass + " host=" + m_host };
        pqxx::work txn(conn);

        // create hypertable using timescaleDB 
        create_hyper_table_ohlcv(txn);

        const auto& metadata = json_data["Meta Data"];
        const std::string symbol = metadata["2. Symbol"];

        const auto& time_series = json_data["Time Series (Daily)"];

        // Insert OHLCV data into the database
        insert_data(txn, time_series, symbol);

        //txn.commit();
        std::cout << "Inserted " << time_series.size() << " OHLCV records for " << symbol << std::endl;

        // check validity of data
        check_data_integrity(txn);
    }
    catch (const std::exception& e)
    {
        std::cerr << "DB error (inserting data): " << e.what() << std::endl;
    }
}

// this will be converted to .sql file once i find a quicker way to i/o. holding this development at a hault for now.
void DatabaseHandler::check_data_integrity(pqxx::work& txn)
{
    try
    {
        // Define all integrity check queries
        const std::vector<std::pair<std::string, std::string>> checks = {
            { "Missing Values Check", R"(
                SELECT COUNT(*) AS bad_entry_count
                FROM ohlcv_data
                WHERE open IS NULL
                   OR high IS NULL
                   OR low IS NULL
                   OR close IS NULL
                   OR volume IS NULL
                   OR time IS NULL;
            )" },
            { "Logical Consistency Check", R"(
                SELECT COUNT(*) AS bad_entry_count
                FROM ohlcv_data
                WHERE NOT (
                    low <= open AND open <= high
                    AND low <= close AND close <= high
                    AND low <= high
                );
            )" },
            { "Reasonable Values Check", R"(
                SELECT COUNT(*) AS bad_entry_count
                FROM ohlcv_data
                WHERE open <= 0
                   OR high <= 0
                   OR low <= 0
                   OR close <= 0
                   OR volume < 0;
            )" },
            { "Duplicate Timestamps Check", R"(
                SELECT COALESCE(SUM(cnt - 1), 0) AS bad_entry_count
                FROM (
                    SELECT COUNT(*) AS cnt
                    FROM ohlcv_data
                    GROUP BY time
                    HAVING COUNT(*) > 1
                ) AS subquery;
            )" },
            { "Duplicate Rows Check", R"(
                SELECT COALESCE(SUM(cnt - 1), 0) AS bad_entry_count
                FROM (
                    SELECT COUNT(*) AS cnt
                    FROM (
                        SELECT md5(open::text || high::text || low::text || close::text || volume::text) AS row_hash
                        FROM ohlcv_data
                    ) AS hashed
                    GROUP BY row_hash
                    HAVING COUNT(*) > 1
                ) AS subquery;
            )" }
        };

        bool all_good = true;

        for (const auto& [check_name, query] : checks)
        {
            pqxx::result r = txn.exec(query);
            int bad_entry_count = r[0]["bad_entry_count"].as<int>();

            std::cout << check_name << ": " << bad_entry_count << " bad entries found." << std::endl;

            if (bad_entry_count > 0)
                all_good = false;
        }

        if (!all_good)
        {
            throw std::runtime_error("Data integrity check failed: some bad entries detected.");
        }
        else
        {
            std::cout << " All integrity checks passed successfully." << std::endl;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "DB error (integrity check): " << e.what() << std::endl;
    }
}