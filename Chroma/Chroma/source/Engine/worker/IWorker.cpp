#include "IWorker.h"



void IWorker::DoWork()
{
	for (auto& job : m_Jobs)
	{
		job();
	}
}

IWorker::IWorker()
{
}


IWorker::~IWorker()
{
}
