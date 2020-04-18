#include "UID.h"




unsigned int UIDRandomChar()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 255);
	return dis(gen);
}

std::string UIDNewUID()
{
	std::stringstream ss;
	for (auto i = 0; i < CHROMA_UID_LENGTH; i++) {
		const auto rc = UIDRandomChar();
		std::stringstream hexstream;
		hexstream << std::hex << rc;
		auto hex = hexstream.str();
		ss << (hex.length() < 2 ? '0' + hex : hex);
	}
	return ss.str();
}


std::string UID::GenerateNewUID()
{
	return UIDNewUID();
}

bool UID::operator<(const UID& rhs) const
{
	return data < rhs.data;
}

UID::UID()
{
	data = GenerateNewUID();
}

UID::UID(std::string UIDData)
{
	data = UIDData;
}
