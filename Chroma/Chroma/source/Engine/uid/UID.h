#ifndef _CHOMA_UID_H_
#define _CHOMA_UID_H_

// generates unique hexadecimal ID
#include <sstream>
#include <random>
#include <string>
#include <ChromaConfig.h>
#include <iostream>

struct UID
{
private:
	std::string GenerateNewUID();
public:
	//friend std::ostream& operator << (std::ostream& out, const UID& c);
	std::string data;
	bool operator <(const UID& rhs) const;
	UID();
	~UID() {};
};

//std::ostream& operator << (std::ostream& out, const UID& uid)
//{
//	out << uid.data;
//	return out;
//}

#endif