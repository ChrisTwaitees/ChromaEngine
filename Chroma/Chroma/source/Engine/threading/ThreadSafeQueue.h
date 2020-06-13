#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_H


#include <common/PrecompiledHeader.h>

template<typename T>
class ThreadSafeQueue
{
public:
	ThreadSafeQueue() {}
	ThreadSafeQueue(const ThreadSafeQueue& copy)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_dataQueue = copy.m_dataQueue;
	}

	void Push(T val)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_dataQueue.push(val);
		m_dataCondVar.notify_one();
	}

	void WaitAndPop(T& val)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_dataCondVar.wait([&m_dataQueue]() { return !m_dataQueue.empty(); });

		val = m_dataQueue.front();
		m_dataQueue.pop();
	}

	std::shared_ptr<T> WaitAndPop()
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_dataCondVar.wait([&m_dataQueue]() { return !m_dataQueue.empty(); });

		std::shared_ptr<T> val(std::make_shared<T>(m_dataQueue.front()));
		m_dataQueue.pop();

		return val;
	}

	bool TryPop(T& val)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		if (m_dataQueue.empty())
		{
			return false;
		}

		val = m_dataQueue.front();
		m_dataQueue.pop();

		return true;
	}

	std::shared_ptr<T> TryPop()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		if (m_dataQueue.empty())
		{
			return std::shared_ptr<T>();
		}

		std::shared_ptr<T> val(std::make_shared<T>(m_dataQueue.front()));
		m_dataQueue.pop();

		return val;
	}

	bool Empty()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		return m_dataQueue.empty();
	}

	unsigned int Size()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		return static_cast<unsigned int>(m_dataQueue.size());
	}

private:
	std::mutex m_mutex;
	std::queue<T> m_dataQueue;
	std::condition_variable m_dataCondVar;
};

#endif //THREAD_SAFE_QUEUE_H