#ifndef _CHOMA_UID_H_
#define _CHOMA_UID_H_

// generates unique hexadecimal ID
#include <sstream>
#include <random>
#include <string>
#include <ChromaConfig.h>

struct UID
{
	std::string GenerateNewUID();
public:
	std::string data;
	bool operator <(const UID& rhs) const;
	UID();
	~UID() {};
};



#endif