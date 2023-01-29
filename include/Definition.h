#pragma once

#include <unordered_map>
#include <string>
#include <Python.h>

namespace MCRCON
{
	struct Server
	{
		std::string		ip;
		unsigned short	port = 0;
	};

	struct Plugin
	{
		PyObject*										PyModule;
		PyObject*										PyClass;
		std::unordered_map<std::string, std::string>	PluginInfo;
	};

	struct GlobalVariable
	{
		std::unordered_map<std::string, Server> servers;
		std::unordered_map<std::string, Plugin> plugins;
	};
}
