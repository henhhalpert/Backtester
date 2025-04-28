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

private:
    // Database connection object.
    //pqxx::connection m_conn;

    std::string m_dbName;
    std::string m_user;
    std::string m_pass;
    std::string m_host;

    // Helper function to create the OHLCV table and hypertable.
    void create_hyper_table_ohlcv(pqxx::work& txn);

    // Helper function to insert OHLCV data from JSON into the database.
    void insert_data(pqxx::work& txn, const nlohmann::json& time_series, const std::string& symbol);

    // data validation after inserting to DB
    void check_data_integrity(pqxx::work& txn);
};

#endif // DATABASE_HANDLER_HPP
