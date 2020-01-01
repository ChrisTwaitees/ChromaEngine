#include "Time.h"


namespace Chroma
{
	double Chroma::Time::m_Current;
	double Chroma::Time::m_Previous;
	double Chroma::Time::m_Delta;
	double Chroma::Time::m_Lag;
	double Chroma::Time::m_MaxMSPerFrame;
	double Chroma::Time::m_FPS;

	void Time::Sleep(int milliseconds)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
	}

	void Time::Init()
	{
		m_MaxMSPerFrame = 1.0f / CHROMA_MAX_FRAME_RATE;
		m_Delta = 0.0f;
		m_Current = 0.0f;
		m_Previous = 0.0f;
		m_Lag = 0.0f;
	}

	float Time::GetLoopingTime(float const& loopDuration)
	{
		return glm::mod((float)GetGameTime(), loopDuration);
	}

	float Time::GetLoopingTimeNormalized(float const& loopDuration)
	{
		return Chroma::Math::Remap01(GetLoopingTime(loopDuration), 0.0, loopDuration);
	}

	void Time::Process()
	{
		// updating deltatime and lag
		m_Current = GetGameTime();
		m_Delta = m_Current - m_Previous;
		m_Previous = m_Current;
		m_Lag += m_Delta;
		m_FPS = 1.0 / m_Delta;
	}
}

