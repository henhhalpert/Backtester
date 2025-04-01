#include "../include/Modules/Data/DataFetcher.hpp"

void DataFetcher::processAsync()
{
    try {
        asio::io_context ioc;

        // Use TLS 1.2 + (instead of deprecated sslv23)
        ssl::context ctx(ssl::context::tlsv12_client);

        // Set default verify paths (uses system-wide CA bundle)
        ctx.set_default_verify_paths();

        // Alpha Vantage API settings  
        std::string host = "www.alphavantage.co";
        std::string port = "443";
        std::string api_key = "0MYR9FADHDJ34QES";
        std::string endpoint = "/query?function=TIME_SERIES_INTRADAY&symbol=IBM&interval=15min&apikey=" + api_key;

        // Set up resolver and SSL stream
        tcp::resolver resolver(ioc);
        ssl::stream<tcp::socket> stream(ioc, ctx);

        // Enable Server Name Indication (SNI)
        if (!SSL_set_tlsext_host_name(stream.native_handle(), host.c_str()))
        {
            throw std::runtime_error("[DataFetcher Error] Failed to set SNI.");
        }

        // Resolve hostname
        auto const results = resolver.resolve(host, port);

        // Connect to the resolved endpoint
        asio::connect(stream.lowest_layer(), results.begin(), results.end());

        // Perform SSL handshake
        stream.handshake(ssl::stream_base::client);

        // Create HTTP request
        http::request<http::string_body> req{ http::verb::get, endpoint, 11 };
        req.set(http::field::host, host);
        req.set(http::field::user_agent, "Boost/1.75");
        req.set(http::field::accept, "application/json");

        // Send HTTP request
        http::write(stream, req);

        // Receive the response
        beast::flat_buffer buffer;
        http::response<http::string_body> res;
        http::read(stream, buffer, res);

        // Close the connection gracefully
        boost::system::error_code ec;
        stream.shutdown(ec);

        // Print HTTP status and response body
        std::string body = res.body();
        /*std::cout << "[HTTP Status] " << res.result_int() << " " << res.reason() << "\n";
        std::cout << "[Raw Response Body] " << body << "\n";*/

        // Check if response is empty
        if (body.empty()) {
            std::cerr << "[DataFetcher Error] Empty response body\n";
            return;
        }

        // Parse JSON response
        nlohmann::json data = nlohmann::json::parse(body);
        std::cout << "[Raw JSON] " << data.dump(2) << "\n";

    }
    catch (const std::exception& e) {
        std::cerr << "[DataFetcher Error] " << e.what() << std::endl;
    }
}

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void DataFetcher::process(nlohmann::json& data)
{
    processAsync();
    CURL* curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        std::string response_string;
        std::string api_key = "0MYR9FADHDJ34QES";
        std::string endpoint = "https://www.alphavantage.co/query?function=TIME_SERIES_INTRADAY&symbol=IBM&interval=15min&apikey=" + api_key;

        // Set the URL
        curl_easy_setopt(curl, CURLOPT_URL, endpoint.c_str());

        // Set write callback to capture response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

        // Enable verbose output to debug SSL/TLS handshake
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        // Perform the request
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
        else {
            std::cout << "[Raw Response Body] " << response_string << std::endl;

            // Parse JSON response
            data = nlohmann::json::parse(response_string);

            // Check if Time Series exists and print the data
            if (data.contains("Time Series (15min)"))
            {
                auto& time_series = data["Time Series (15min)"];
                auto it = time_series.begin();
                if (it != time_series.end())
                {
                    std::cout << "[DataFetcher] " << "IBM price at " << it.key() << ": "
                        << it.value()["1. open"] << "\n";
                }
            }
            else
            {
                std::cerr << "[DataFetcher] API Error: " << data.dump(2) << "\n";
            }
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}