#include "UID.h"




unsigned int UIDRandomChar()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 255);
	return dis(gen);
}

const char* UIDNewUID()
{
	std::stringstream ss;
	for (auto i = 0; i < CHROMA_UID_LENGTH; i++) {
		const auto rc = UIDRandomChar();
		std::stringstream hexstream;
		hexstream << std::hex << rc;
		auto hex = hexstream.str();
		ss << (hex.length() < 2 ? '0' + hex : hex);
	}
	return ss.str().c_str();
}


const char* UID::GenerateNewUID()
{
	return UIDNewUID();
}