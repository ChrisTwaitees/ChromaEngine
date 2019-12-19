#include "IWorker.h"



void IWorker::DoWork()
{
	for (auto& job : m_Jobs)
	{
		job();
	}
	std::cout << "num workers : " << m_Jobs.size() << std::endl;
}

IWorker::IWorker()
{
}


IWorker::~IWorker()
{
}
