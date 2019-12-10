#ifndef _CHROMA_UID_
#define _CHROMA_UID_
// generates unique hexadecimal ID
#include <sstream>
#include <random>
#include <string>


class ChromaUID
{
private:
	static unsigned int random_char();
	static const unsigned int CHROMA_UID_LENGTH{ 8 };
	static std::string NewID();
public:
	std::string UID;
	ChromaUID();
	~ChromaUID();
};

#endif