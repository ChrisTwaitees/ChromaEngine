#include "JobSystemTest.h"

std::mutex JobSystemTest::m_Mutex;
std::vector<std::future<void>> JobSystemTest::m_Futures;

void JobSystemTest::SendJob(void(*func)(std::string), std::string path)
{
	std::lock_guard<std::mutex> lock(m_Mutex);
	m_Futures.push_back(std::async(std::launch::async,  func, path));
}
