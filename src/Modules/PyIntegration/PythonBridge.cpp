#include "../include/Modules/PyIntegration/PythonBridge.hpp"

PythonBridge::PythonBridge() :
	m_guard{}
{		
	m_sys = py::module::import("sys");
	m_sys.attr("path").attr("insert")(0, PYTHON_DIR);
	m_runner = py::module::import(PYTHON_RUNNER);
}

py::error_already_set PythonBridge::CallFunction(const std::string& taskName)
{
	try
	{
		m_runner.attr("run")(taskName);
	}
	catch (const py::error_already_set& err)
	{
		std::cerr << "PythonBridge::CallFunction(const std::string&) Error: " << err.what() << std::endl;
	}
	return py::error_already_set();
}
