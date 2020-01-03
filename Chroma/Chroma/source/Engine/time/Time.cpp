#include "Time.h"


namespace Chroma
{
	double Time::m_Current;
	double Time::m_Previous;
	double Time::m_Delta;
	double Time::m_Lag;
	double Time::m_MaxFrameTime;
	double Time::m_FPS;
	float Time::m_Speed;

	void Time::Sleep(int milliseconds)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
	}

	void Time::Init()
	{
		m_MaxFrameTime = 1.0f / CHROMA_MAX_FRAME_RATE;
		m_Delta = 0.0f;
		m_Current = 0.0f;
		m_Previous = 0.0f;
		m_Lag = 0.0f;
		m_Speed = 1.0f;
	}

	float Time::GetLoopingTime(float const& loopDuration)
	{
		return glm::mod((float)GetGameTime(), loopDuration);
	}

	float Time::GetLoopingTimeNormalized(float const& loopDuration)
	{
		return Chroma::Math::Remap01(GetLoopingTime(loopDuration), 0.0, loopDuration);
	}

	void Time::Update()
	{
		// updating deltatime and lag
		m_Current = GetGameTime();
		m_Delta = m_Current - m_Previous;
		m_Previous = m_Current;
		m_Lag += m_Delta;
		m_FPS = 1.0 / m_Delta;
	}
}

