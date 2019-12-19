#ifndef _CHROMA_WORKER_H_
#define	_CHROMA_WORKER_H_


#include <vector>
#include <future>
#include <iostream>



//int func()
//{
//	int some_value = 0;
//	// Do work...
//	return some_value;
//}
//int main()
//{
//	std::future<int> result_future = std::async(func);
//	// Do something...
//	int result = result_future.get();
//}


static std::vector<std::function<void(void)>> m_Jobs;

class IWorker
{
//	typedef std::vector<std::function<void(void)>> m_Functions;

public:

	static void AddWork(std::function<void(void)>& newFunction) { m_Jobs.push_back(newFunction); };

	void AddJob(std::function<void(void)>& newFunction) { m_Jobs.push_back(newFunction); };
	static void DoWork();

	IWorker();
	~IWorker();
};

#endif