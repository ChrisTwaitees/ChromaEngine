#ifndef _CHOMA_UID_H_
#define _CHOMA_UID_H_

// generates unique hexadecimal ID
//common
#include <common/PrecompiledHeader.h>

#include <ChromaConfig.h>
#include <iostream>

struct UID
{
private:
	unsigned int GenerateNewUID();
	static unsigned int GlobalUIDCount;
public:
	//friend std::ostream& operator << (std::ostream& out, const UID& c);
	static void ResetGlobalUIDs() { UID::GlobalUIDCount = 0; }
	unsigned int m_Data;
	bool operator <(const UID& rhs) const { return this->m_Data < rhs.m_Data; };
	bool operator ==(const UID& rhs) const { return this->m_Data == rhs.m_Data; }
	UID();
	UID(unsigned int UIDData) : m_Data(UIDData) { };
	UID(std::string UIDData) : m_Data(std::stoi(UIDData)) { };
	UID(const char* UIDData) : m_Data(std::stoi(UIDData)) { };
	~UID() {};
};

//std::ostream& operator << (std::ostream& out, const UID& uid)
//{
//	out << uid.data;
//	return out;
//}

#endif