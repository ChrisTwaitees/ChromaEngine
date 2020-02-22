#ifndef _CHROMA_LOG_
#define _CHROMA_LOG_

//common
#include <common/PrecompiledHeader.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Chroma {

	class Log
	{
	private:
		 
		static std::shared_ptr<spdlog::logger> m_CoreLogger;

	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return m_CoreLogger; }

	};
}


#endif

#if defined(RELEASE)
// Core log macros
#define CHROMA_TRACE(...)      ::Chroma::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CHROMA_TRACE_UNDERLINE ::Chroma::Log::GetCoreLogger()->trace("------------------------------------------")
#define CHROMA_INFO(...)       ::Chroma::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CHROMA_INFO_UNDERLINE  ::Chroma::Log::GetCoreLogger()->info("------------------------------------------")
#define CHROMA_WARN(...)       ::Chroma::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CHROMA_WARN_UNDERLINE  ::Chroma::Log::GetCoreLogger()->warn("------------------------------------------")
#define CHROMA_ERROR(...)      ::Chroma::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CHROMA_FATAL(...) 	   ::Chroma::Log::GetCoreLogger()->error(__VA_ARGS__)


#elif defined(DEBUG)
// Core log macros
#define CHROMA_TRACE(...)      ::Chroma::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CHROMA_TRACE_UNDERLINE ::Chroma::Log::GetCoreLogger()->trace("------------------------------------------")
#define CHROMA_INFO(...)       ::Chroma::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CHROMA_INFO_UNDERLINE  ::Chroma::Log::GetCoreLogger()->info("------------------------------------------")
#define CHROMA_WARN(...)       ::Chroma::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CHROMA_WARN_UNDERLINE  ::Chroma::Log::GetCoreLogger()->warn("------------------------------------------")
#define CHROMA_ERROR(...)      ::Chroma::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CHROMA_FATAL(...) 	   ::Chroma::Log::GetCoreLogger()->error(__VA_ARGS__)

#endif