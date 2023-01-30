#include "Initialization.h"

#include <string>
#include <filesystem>
#include <iostream>
#include <format>
#include <thread>

#include <Python.h>

namespace MCRCON
{
	Initialization::Initialization(MCRCON::GlobalVariable& v)
		:	infile("./config.json"),
			variable(v)
	{
		//python初始化
		Py_Initialize();
		PyRun_SimpleString("import sys");
		PyRun_SimpleString("import os");
		PyRun_SimpleString("sys.path.append('{}/Plugin'.format(os.getcwd()))");

		PythonSetting();
	}

	Initialization::~Initialization()
	{
		//python结束
		Py_Finalize();
	}

	void Initialization::init()
	{
		if (!infile)
		{
			makeConfigFile();
			std::cout << "请填写 ./config.json\n";
			exit(-1);
		}

		if (!std::filesystem::exists("./Plugin"))
		{
			std::filesystem::create_directory("./Plugin");
		}

		//读取./config.json
		if (loadConfigFile())
		{
			reloadServer();
		}

		reloadPlugin();
	}

	//重新加载服务器
	void Initialization::reloadServer()
	{
		if (loadConfigFile())
		{
			std::cout << "读取失败\n";
			return;
		}

		std::unordered_map<std::string, MCRCON::Server> servers = variable.servers;
		auto port = variable.MCRconAddress.port;
		if (loadConfig(config, servers, port))
		{
			std::cout << "读取失败\n";
			return;
		}
		variable.servers = servers;
	}

	//重新加载插件
	void Initialization::reloadPlugin()
	{
		std::filesystem::path pluginPath("./Plugin");
		std::filesystem::directory_iterator pluginPathItor(pluginPath);

		for (; !pluginPathItor._At_end(); pluginPathItor++)
		{
			if (pluginPathItor->is_directory())
			{
				continue;
			}
			if (pluginPathItor->path().extension() == ".py")
			{
				auto localName = pluginPathItor->path().filename().string();
				std::cout << "正在加载" << localName << '\n';

				//读取模块
				PyObject* pyModule = PyImport_ImportModule(localName.substr(0, localName.size() - 3).c_str());
				if (pyModule == nullptr)
				{
					std::cout << "加载" << localName << "失败\n";
					continue;
				}

				//读取模块中的函数列
				PyObject* pyModuleDict = PyModule_GetDict(pyModule);
				if (pyModuleDict == nullptr)
				{
					std::cout << "加载" << localName << "的函数列失败\n";
					continue;
				}

				//读取PluginInfo
				PyObject* PluginInfo = PyDict_GetItemString(pyModuleDict, "PluginInfo");
				if (pyModuleDict == nullptr)
				{
					std::cout << "加载" << localName << "的插件信息失败\n";
					continue;
				}

				//判断是否是dict
				if (PyDict_Check(PluginInfo))
				{
					//获取Name
					const char* PluginName = _PyUnicode_AsString(PyDict_GetItemString(PluginInfo, "Name"));
					if (PluginName == nullptr)
					{
						std::cout << "加载" << localName << "的插件名字失败\n";
						continue;
					}
					variable.plugins[PluginName].PluginInfo["Name"] = PluginName;

					//获取Start
					const char* StartName = _PyUnicode_AsString(PyDict_GetItemString(PluginInfo, "Start"));
					if (StartName == nullptr)
					{
						std::cout << "加载" << localName << "的StartClass名失败\n";
						continue;
					}
					variable.plugins[PluginName].PluginInfo["Start"] = StartName;

					//获取Version
					const char* VersionName = _PyUnicode_AsString(PyDict_GetItemString(PluginInfo, "Version"));
					if (VersionName == nullptr)
					{
						std::cout << "加载" << localName << "的版本号失败\n";
						continue;
					}
					variable.plugins[PluginName].PluginInfo["Version"] = VersionName;

					PyObject* Start = PyInstanceMethod_New(PyDict_GetItemString(pyModuleDict, StartName));
					if (Start == nullptr)
					{
						std::cout << "加载" << localName << "的Class失败\n";
						continue;
					}
					variable.plugins[PluginName].PyClass = Start;
					variable.plugins[PluginName].PyModule = pyModule;

					if (PyObject_CallMethod(Start, "onLoad", "(O)", Start) == nullptr)
					{
						std::cout << localName << " 的 onLoad 调用失败\n";
						continue;
					}

					std::cout << "插件 " << PluginName << " v" << VersionName << " 加载成功\n";
				}
				else
				{
					std::cout << "PluginInfo不是dict类型\n";
				}
			}
		}

		for (auto i = variable.plugins.begin(); i != variable.plugins.end(); i++)
		{
			if (PyObject_CallMethod(i->second.PyClass, "onEnable", "(O)", i->second.PyClass) == nullptr)
			{
				std::cout << i->second.PluginInfo["Name"] << " 的 onEnable 调用失败\n";
				continue;
			}
		}
	}

	//给Python设置环境
	void Initialization::PythonSetting()
	{
		PyRun_SimpleString("import QQOF");
		PyRun_SimpleString("QQOF.locLogger = QQOF.Logger()");
		PyRun_SimpleString("QQOF.locLogger.init(os.path.join(os.getcwd(), './logs/'))");
	}

	//加载config文件
	int Initialization::loadConfigFile()
	{
		try
		{
			infile.seekg(0, std::ios_base::end);
			size_t size = infile.tellg();
			infile.seekg(0, std::ios_base::beg);
			std::string buffer;
			buffer.resize(size);
			infile.read(buffer.data(), size);
			infile.close();

			//读取
			config = qjson::JParser::fastParse(buffer);
		}
		catch (const std::exception& e)
		{
			return -1;
		}

		//返回正确码
		return 0;
	}

	int Initialization::loadConfig(qjson::JObject& config,
		std::unordered_map<std::string, MCRCON::Server>& servers,
		unsigned short& serverPort
	)
	{
		//将config中的servers加载到内存中
		try
		{
			servers.clear();
			qjson::dict_t& dict = config["servers"].getDict();
			for (auto i = dict.begin(); i != dict.end(); i++)
			{
				servers[i->first].ip = i->second["ip"];
				servers[i->first].port = static_cast<unsigned short>(
					static_cast<int>(i->second["rconport"])
					);
			}

			serverPort = static_cast<unsigned short>(
				static_cast<int>(config["MCRCON"]["port"])
				);
			
		}
		catch (...)
		{
			return -1;
		}

		return 0;
	}

	//创建config文件
	void Initialization::makeConfigFile()
	{
		qjson::JObject json;

		//示例
		json["servers"]["lobby"]["ip"] = "127.0.0.1";
		json["servers"]["lobby"]["rconport"] = 25575;
		json["MCRCON"]["port"] = 2374;

		json["plugin"] = "./Plugin";

		std::ofstream outfile("./config.json");
		outfile << qjson::JWriter::fastFormatWrite(json);
	}
}
