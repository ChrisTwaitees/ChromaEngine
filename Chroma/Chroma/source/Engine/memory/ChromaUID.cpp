#include "ChromaUID.h"

unsigned int ChromaUID::random_char()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 255);
	return dis(gen);
}

std::string ChromaUID::NewID()
{
	std::stringstream ss;
	for (auto i = 0; i < CHROMA_UID_LENGTH; i++) {
		const auto rc = random_char();
		std::stringstream hexstream;
		hexstream << std::hex << rc;
		auto hex = hexstream.str();
		ss << (hex.length() < 2 ? '0' + hex : hex);
	}
	return ss.str();
}

ChromaUID::ChromaUID()
{
	UID = NewID();
}


ChromaUID::~ChromaUID()
{
}