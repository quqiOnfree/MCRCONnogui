#include "MessageHeader.h"

namespace mhead
{
	MessageHeader::MessageHeader()
	{
	}

	MessageHeader::MessageHeader(const std::string& data)
	{
		p_data = data;
	}

	MessageHeader::MessageHeader(std::string&& data)
	{
		p_data = std::move(data);
	}

	void MessageHeader::write(const std::string& data)
	{
		p_data += data;
	}

	bool MessageHeader::canRead() const
	{
		size_t dataSize = 0;

		if (p_data.size() < sizeof(dataSize))
			return false;
		
		memcpy_s((char*)&dataSize, sizeof(dataSize), p_data.c_str(), sizeof(dataSize));

		if (dataSize + sizeof(dataSize) <= p_data.size())
			return true;
		return false;
	}

	size_t MessageHeader::readSize() const
	{
		size_t dataSize = 0;
		memcpy_s((char*)&dataSize, sizeof(dataSize), p_data.c_str(), sizeof(dataSize));

		return dataSize;
	}

	size_t MessageHeader::allSize() const
	{
		return p_data.size();
	}

	std::string MessageHeader::read()
	{
		if (!canRead())
			throw std::exception("can't read");

		size_t dataSize = 0;
		memcpy_s((char*)&dataSize, sizeof(dataSize), p_data.c_str(), sizeof(dataSize));

		std::string reData(std::move(p_data.substr(sizeof(dataSize), dataSize)));
		p_data = std::move(p_data.substr(sizeof(dataSize) + dataSize));
		return std::move(reData);
	}

	MessageHeader& MessageHeader::operator=(MessageHeader&& MH) noexcept
	{
		if (this == &MH)
			return *this;

		p_data = std::move(MH.p_data);
		return *this;
	}

	MessageHeader& MessageHeader::operator=(const MessageHeader& MH)
	{
		if (this == &MH)
			return *this;

		p_data = MH.p_data;
		return *this;
	}

	std::string makeMessageHeader(const std::string& data)
	{
		size_t dataSize = data.size();
		std::string locmsg((char*)&dataSize, sizeof(dataSize));

		return locmsg + data;
	}

	std::string makeMessageHeader(std::string&& data)
	{
		size_t dataSize = data.size();
		std::string locmsg((char*)&dataSize, sizeof(dataSize));

		return locmsg + std::move(data);
	}
}