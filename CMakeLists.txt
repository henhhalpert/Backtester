cmake_minimum_required(VERSION 3.10)

project(Backtester VERSION 1.0)
set(CMAKE_CXX_STANDARD 17)

# Auto-collect files
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/include/*.hpp")
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")

# Debugging: Print collected source and header files
message(STATUS "Found source files: ${SOURCES}")
message(STATUS "Found header files: ${HEADERS}")

# Add executable (add .cpp files + .hpp files)
add_executable(Backtester ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(Backtester PUBLIC include)
target_include_directories(Backtester PRIVATE ${PostgreSQL_INCLUDE_DIRS})

# Manually set headers as header-only for Visual Studio
foreach(header ${HEADERS})
    set_source_files_properties(${header} PROPERTIES HEADER_FILE_ONLY ON)
endforeach()

# packages 
find_package(Boost REQUIRED COMPONENTS asio beast json system)
find_package(OpenSSL REQUIRED)
find_package(CURL REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(libpqxx CONFIG REQUIRED)

# Link Boost libraries to your executable
target_link_libraries(Backtester PRIVATE 
    Boost::asio 
    Boost::beast 
    Boost::json 
    Boost::system 
    OpenSSL::SSL 
    OpenSSL::Crypto 
    CURL::libcurl 
    nlohmann_json::nlohmann_json
    libpqxx::pqxx
)

# Proper folder grouping for .cpp and .hpp files
source_group(TREE "${CMAKE_SOURCE_DIR}/src" PREFIX "src" FILES ${SOURCES})
source_group(TREE "${CMAKE_SOURCE_DIR}/include" PREFIX "include" FILES ${HEADERS})
