#ifndef _CHROMA_LOG_
#define _CHROMA_LOG_

//common
#include <common/PrecompiledHeader.h>

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Chroma {

	class Log
	{
	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return m_CoreLogger; }
	private:
		static std::shared_ptr<spdlog::logger> m_CoreLogger;
	};
}


#endif

#if defined(RELEASE)
// Core log macros
#define CHROMA_TRACE(...)      
#define CHROMA_TRACE_UNDERLINE 
#define CHROMA_INFO(...)       
#define CHROMA_INFO_UNDERLINE  
#define CHROMA_WARN(...)       
#define CHROMA_WARN_UNDERLINE  
#define CHROMA_ERROR(...)      
#define CHROMA_FATAL(...) 	   
// asserts
#define CHROMA_DEBUGBREAK()
#define CHROMA_ASSERT(x, ...) 


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
// asserts
#define CHROMA_DEBUGBREAK() __debugbreak()
#define CHROMA_ASSERT(x, ...) { if(!(x)) { CHROMA_ERROR("Assertion Failed: {0}", __VA_ARGS__); CHROMA_DEBUGBREAK(); } }


#endif