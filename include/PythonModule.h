#pragma once

#include <asio.hpp>
#include <string>
#include <string_view>

#include "Definition.h"

namespace MCRCON
{
	class PythonModule
	{
	public:
		PythonModule(MCRCON::GlobalVariable&);

		asio::awaitable<void> sendMessage(std::string_view data);

	private:
		MCRCON::GlobalVariable& p_GlobalVariable;
	};
}
