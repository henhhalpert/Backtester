#pragma once

// i/o
#include <iostream>
#include <cstdlib>

// containers 
#include <vector>
#include <string>
#include <map>

// boost
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/version.hpp>
namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

// json
#include <nlohmann/json.hpp>




