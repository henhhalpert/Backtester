cmake_minimum_required(VERSION 3.10)
project(Backtester VERSION 1.0)
set(CMAKE_CXX_STANDARD 20)

add_definitions(-D_WIN32_WINNT=0x0A00)


# == Packages ==
# Boost packages
find_package(Boost REQUIRED asio beast system)

# OpenSSL packages
find_package(OpenSSL REQUIRED)

# CURL packages
find_package(CURL REQUIRED)

# JSON package
find_package(nlohmann_json CONFIG REQUIRED)

# PostgreSQL packages
find_package(PostgreSQL REQUIRED)
find_package(libpqxx CONFIG REQUIRED)

# Python3 integration
#find_package(Python3 COMPONENTS Interpreter Development)

# pybind11 package
find_package(pybind11 CONFIG REQUIRED)

# == Explicitly Add Source and Header Files ==
set(SOURCES
    src/Modules/Core/AppInitializer.cpp
    src/Modules/Core/AppRunner.cpp
    src/Modules/Data/DataCleaner.cpp
    src/Modules/Data/DataFetcher.cpp
    src/Modules/Data/DataPipelineManager.cpp
    src/Modules/Data/DataTransformer.cpp
    src/Modules/Data/DatabaseHandler.cpp
    src/Modules/PyIntegration/PythonBridge.cpp
    src/Modules/Utils/SystemUtils.cpp
    src/main.cpp
)

set(HEADERS
    include/Logger.hpp
    include/Modules/Core/AppInitializer.hpp
    include/Modules/Core/AppRunner.hpp
    include/Modules/Data/DataCleaner.hpp
    include/Modules/Data/DataFetcher.hpp
    include/Modules/Data/DataPipelineManager.hpp
    include/Modules/Data/DataTransformer.hpp
    include/Modules/Data/DatabaseHandler.hpp
    include/Modules/Data/IDataStage.hpp
    include/Modules/PyIntegration/PythonBridge.hpp
    include/Modules/Utils/SystemUtils.hpp
    include/pch.hpp
)

# Add executable (add .cpp files + .hpp files)
add_executable(Backtester ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(Backtester PUBLIC include)
target_include_directories(Backtester PRIVATE ${PostgreSQL_INCLUDE_DIRS})

# packages 
find_package(Boost REQUIRED COMPONENTS asio beast json system)
find_package(OpenSSL REQUIRED)
find_package(CURL REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(libpqxx CONFIG REQUIRED)
find_package(pybind11 REQUIRED)

# python binding
set(VCPKG_CHAINLOAD_TOOLCHAIN_FILE OFF)
set(Python3_ROOT_DIR "C:/Program Files/Python313")
set(Python3_EXECUTABLE "C:/Program Files/Python313/python.exe")
set(Python3_INCLUDE_DIR "C:/Program Files/Python313/include")
set(Python3_LIBRARY "C:/Program Files/Python313/libs/python313.lib")
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)

# Link Boost libraries to your executable
target_link_libraries(Backtester PRIVATE 
    Boost::asio 
    Boost::beast 
    Boost::system 
    OpenSSL::SSL 
    OpenSSL::Crypto 
    CURL::libcurl 
    nlohmann_json::nlohmann_json
    libpqxx::pqxx
    Python3::Python
    pybind11::embed
)

# Proper folder grouping for .cpp and .hpp files
source_group(TREE "${CMAKE_SOURCE_DIR}/src" PREFIX "src" FILES ${SOURCES})
source_group(TREE "${CMAKE_SOURCE_DIR}/include" PREFIX "include" FILES ${HEADERS})
