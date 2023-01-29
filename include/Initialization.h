#pragma once

#include <fstream>

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
		static int loadConfig(qjson::JObject&, MCRCON::GlobalVariable&);
		void makeConfigFile();

	private:
		std::ifstream			infile;
		qjson::JObject			config;
		MCRCON::GlobalVariable& variable;
	};
}
