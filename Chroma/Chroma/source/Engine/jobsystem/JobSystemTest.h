#pragma once

#include <common/PrecompiledHeader.h>
#include <vector>

class JobSystemTest
{
public:
	static void SendJob(void(*func)(std::string), std::string path);

private:
	static std::mutex m_Mutex;
	static std::vector<std::future<void>> m_Futures;
};

