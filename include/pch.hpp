#pragma once

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

// python 
#include <pybind11/embed.h>