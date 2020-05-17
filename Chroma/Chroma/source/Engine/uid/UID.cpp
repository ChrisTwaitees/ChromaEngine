#include "UID.h"


unsigned int UID::GlobalUIDCount{ 0 };

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
	for (unsigned int i = 0; i < CHROMA_UID_LENGTH; i++) {
		unsigned int rc = UIDRandomChar();
		std::stringstream hexstream;
		hexstream << std::hex << rc;
		auto hex = hexstream.str();
		ss << (hex.length() < 2 ? '0' + hex : hex);
	}
	return ss.str().c_str();
}


unsigned int UID::GenerateNewUID()
{
	GlobalUIDCount++;
	return UID::GlobalUIDCount;
}


UID::UID()
{
	m_Data = GenerateNewUID();
}
