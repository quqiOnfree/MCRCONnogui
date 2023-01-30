#pragma once

#include <fstream>
#include <asio.hpp>

#include "Definition.h"
#include "Json.h"

namespace MCRCON
{
	class Initialization
	{
	public:
		Initialization(MCRCON::GlobalVariable&);
		~Initialization();
		void init();

		void reloadServer();
		void reloadPlugin();

	protected:
		void PythonSetting();

		int loadConfigFile();

		static int loadConfig(	qjson::JObject&,
								std::unordered_map<std::string, MCRCON::Server>&,
								unsigned short&
		);

		void makeConfigFile();

	private:
		std::ifstream			infile;
		qjson::JObject			config;
		MCRCON::GlobalVariable& variable;
	};
}
