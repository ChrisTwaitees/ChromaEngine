#ifndef _CHROMA_LOG_
#define _CHROMA_LOG_

#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Chroma {

	class Log
	{
	private:
		 
		static std::shared_ptr<spdlog::logger> m_CoreLogger;
		static std::shared_ptr<spdlog::logger> m_ClientLogger;

	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return m_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return m_ClientLogger; }

	};
}


#endif

#if defined(RELEASE)

// Core log macros
#define CHROMA_CORE_TRACE(...)    ::Chroma::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CHROMA_CORE_INFO(...)     ::Chroma::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CHROMA_CORE_WARN(...)     ::Chroma::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CHROMA_CORE_ERROR(...)    ::Chroma::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CHROMA_CORE_FATAL(...)    ::Chroma::Log::GetCoreLogger()->error(__VA_ARGS__)

// Core log macros
#define CHROMA_TRACE(...)         ::Chroma::Log::GetClientLogger()->trace(__VA_ARGS__)
#define CHROMA_INFO(...)          ::Chroma::Log::GetClientLogger()->info(__VA_ARGS__)
#define CHROMA_WARN(...)          ::Chroma::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CHROMA_ERROR(...)         ::Chroma::Log::GetClientLogger()->error(__VA_ARGS__)
#define CHROMA_FATAL(...)         ::Chroma::Log::GetClientLogger()->error(__VA_ARGS__)


#elif defined(DEBUG)
// Core log macros
#define CHROMA_CORE_TRACE(...) 
#define CHROMA_CORE_INFO(...)  
#define CHROMA_CORE_WARN(...)  
#define CHROMA_CORE_ERROR(...) 
#define CHROMA_CORE_FATAL(...) 

// Core log macros
#define CHROMA_TRACE(...)      
#define CHROMA_INFO(...)       
#define CHROMA_WARN(...)       
#define CHROMA_ERROR(...)      
#define CHROMA_FATAL(...) 

#endif