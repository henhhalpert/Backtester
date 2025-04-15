#pragma once
#include <pch.hpp>

enum class PythonTasks;
inline const char* pythonTaskToString(PythonTasks task);

//! Might add gRPC support in the future, and make this python module an actual client. For now, it will suffice. 
class PythonBridge
{
public:
	PythonBridge();
	~PythonBridge() = default;

	PythonBridge(const PythonBridge&) = delete;
	PythonBridge& operator=(const PythonBridge&) = delete;

	PythonBridge(PythonBridge&&) noexcept = default;
	PythonBridge& operator=(PythonBridge&&) noexcept = default;

	py::error_already_set CallFunction(const std::string& taskName);
private:
	// init py using scoped guard 
	py::scoped_interpreter m_guard;
	// basic module that will be used to import and run .py 
	py::module m_sys;
	// a routing object that will call runner.py, runner.py calls other functions that run in other .py modules. 
	py::module m_runner;
};

enum class PythonTasks
{
	EDA = 0
};

inline const char* pythonTaskToString(PythonTasks task)
{
	switch (task)
	{
	case PythonTasks::EDA:                  return "eda";
	default:                                return "unknown";
	}
}