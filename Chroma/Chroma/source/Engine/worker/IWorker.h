#ifndef _CHROMA_WORKER_H_
#define	_CHROMA_WORKER_H_


#include <vector>
#include <future>
#include <iostream>

// Consider updating to allow asynchronous jobs

static std::vector<std::function<void(void)>> m_Jobs;

class IWorker
{
public:

	void AddJob(std::function<void(void)>& newFunction) { m_Jobs.push_back(newFunction); };
	static void DoWork();

	IWorker();
	~IWorker();
};

#endif