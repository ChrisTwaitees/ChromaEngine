#include "Log.h"


namespace Chroma {

	std::shared_ptr<spdlog::logger> Log::m_CoreLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		m_CoreLogger = spdlog::stdout_color_mt("Chroma");
		m_CoreLogger->set_level(spdlog::level::trace);
		CHROMA_INFO("Logging Initialized.");
	}
}
