#pragma once

// general macros  
constexpr auto DB_NAME			= "DB_NAME";
constexpr auto DB_USER			= "DB_USER";
constexpr auto DB_PASS			= "DB_PASSWORD";
constexpr auto DB_HOST			= "DB_HOST";

// Python 
constexpr auto PYTHON_DIR		= "C:\\Dev\\Backtester\\scripts";
// the runner.py is responsible for routing function calls within different .py modules.
constexpr auto PYTHON_RUNNER	= "runner";

constexpr auto PYTHONPATH		= "PYTHONPATH";
constexpr auto SYS_BUFFER_SIZE  = 256;

// i/o
#include <iostream>
#include <cstdlib>

// containers 
#include <vector>
#include <string>
#include <map>

// boost
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/json.hpp>
#include <boost/asio/ssl.hpp>
namespace asio = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;
namespace ssl = asio::ssl;
using tcp = asio::ip::tcp;

// curl 
#include <curl/curl.h>

// json 
#include <nlohmann/json.hpp>

// postgres 
#include <pqxx/pqxx>

// python binds 
#include <python.h>
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
namespace py = pybind11;