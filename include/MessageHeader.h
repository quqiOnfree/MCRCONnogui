#pragma once

#include <string>

namespace mhead
{
	class MessageHeader
	{
	public:
		MessageHeader();
		MessageHeader(const std::string& data);
		MessageHeader(std::string&& data);
		void write(const std::string& data);
		bool canRead() const;
		size_t readSize() const;
		size_t allSize() const;
		std::string read();
		MessageHeader& operator=(MessageHeader&& MH) noexcept;
		MessageHeader& operator=(const MessageHeader& MH);
	private:
		std::string		p_data;
	};

	std::string makeMessageHeader(const std::string& data);
	std::string makeMessageHeader(std::string&& data);
}



