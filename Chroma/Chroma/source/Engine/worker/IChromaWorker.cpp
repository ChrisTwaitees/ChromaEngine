#include "IChromaWorker.h"



void IChromaWorker::DoWork()
{
	for (auto& job : m_Jobs)
	{
		job();
	}
	std::cout << "num workers : " << m_Jobs.size() << std::endl;
}

IChromaWorker::IChromaWorker()
{
}


IChromaWorker::~IChromaWorker()
{
}
