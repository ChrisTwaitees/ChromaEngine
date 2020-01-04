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
	std::set<std::reference_wrapper<float>> Time::m_Timers;


	void Time::Sleep(unsigned int milliseconds)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
	}

	void Time::ProcessTimers()
	{
		std::set<std::reference_wrapper<float>>::iterator it = m_Timers.begin();

		for (float& timer : m_Timers)
		{
			if (timer - m_Delta < 0.0f)
			{
				timer = 0.0f;
				m_Timers.erase(it);
			}
			else
			{
				timer -= m_Delta;
				CHROMA_INFO("Number of timers : {0}", m_Timers.size());
			}
		}
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
		// Calculate Time
		m_Current = GetGameTime();
		m_Delta = m_Current - m_Previous;
		m_Previous = m_Current;
		m_Lag += m_Delta;
		m_FPS = 1.0 / m_Delta;

		// Process Timers if any exist
		ProcessTimers();
	}
}

