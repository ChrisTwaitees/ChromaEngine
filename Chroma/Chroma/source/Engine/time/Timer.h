#ifndef CHROMA_TIMER_H
#define CHROMA_TIMER_H

#include <common/PrecompiledHeader.h>

namespace Chroma
{
	struct LogScopeTimer
	{
		LogScopeTimer(const std::string& name) : m_Name(name) { Init(); };
		~LogScopeTimer() { Destroy(); };
	private:
		void Init()
		{
			m_StartTime = std::chrono::high_resolution_clock::now();
		}
		void Destroy()
		{
			std::chrono::steady_clock::time_point end = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - m_StartTime);
			CHROMA_TRACE("{0} took : {1} ms", m_Name, (float)duration.count() );
		}

		std::string m_Name{ "" };

		std::chrono::steady_clock::time_point m_StartTime;


	};
}

#define CHROMA_TRACE_TIMER_SCOPE(...)	LogScopeTimer __timer = Chroma::LogScopeTimer(__VA_ARGS__);

#endif