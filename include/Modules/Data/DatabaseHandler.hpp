#ifndef DATABASE_HANDLER_HPP
#define DATABASE_HANDLER_HPP

#include "IDataStage.hpp"

class DatabaseHandler : public IDataStage
{
public:
    DatabaseHandler() = delete;

    // Constructor: Initializes the database connection parameters.
    DatabaseHandler(const std::string& db_name, const std::string& user, const std::string& password, const std::string& host = "localhost");

    // Destructor: Closes the database connection.
    ~DatabaseHandler();

    // Disable copy ctor and copy assignment operator
    DatabaseHandler(const DatabaseHandler&) = delete;
    DatabaseHandler& operator=(const DatabaseHandler&) = delete;

    // Disable move ctor and move assignment operator
    DatabaseHandler(DatabaseHandler&&) = delete;
    DatabaseHandler& operator=(DatabaseHandler&&) = delete;

    // Inserts OHLCV data into the database from a JSON object.
    void process(nlohmann::json& json_data) override;

    // fetch data from DB
    std::vector<std::tuple<std::string, double, double, double, double>> fetch_ohlcv_for_plot(
        const std::string& symbol, const std::string& start_time, const std::string& end_time);

private:
    // Database connection object.
    pqxx::connection conn_;

    // Helper function to create the OHLCV table and hypertable.
    void create_hyper_table_ohlcv(pqxx::work& txn);

    // Helper function to insert OHLCV data from JSON into the database.
    void insert_data(pqxx::work& txn, const nlohmann::json& time_series, const std::string& symbol);

};

#endif // DATABASE_HANDLER_HPP
