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
	std::vector<NormalizedTimer10> Time::m_NormalizedTimers10;
	std::vector<NormalizedTimer01> Time::m_NormalizedTimers01;


	void Time::Sleep(unsigned int milliseconds)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
	}

	void Time::ProcessTimers()
	{
		// timers
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
			}
		}

		// normalizedTimers10
		std::vector<NormalizedTimer10>::iterator normIt10 = m_NormalizedTimers10.begin();

		for (NormalizedTimer10& timer : m_NormalizedTimers10)
		{
			if (timer.m_CurrentTime - m_Delta < 0.0f)
			{
				*timer.m_Timer = 0.0f;
				m_NormalizedTimers10.erase(normIt10);
			}
			else
			{
				timer.m_CurrentTime -= m_Delta;
				*timer.m_Timer = timer.m_CurrentTime * (1.0/timer.m_StartTime);
			}
		}

		// normalizedTimers01
		std::vector<NormalizedTimer01>::iterator normIt01 = m_NormalizedTimers01.begin();

		for (NormalizedTimer01& timer : m_NormalizedTimers01)
		{
			if (timer.m_CurrentTime - m_Delta < 0.0f)
			{
				*timer.m_Timer = 1.0f;
				m_NormalizedTimers01.erase(normIt01);
			}
			else
			{
				timer.m_CurrentTime -= m_Delta;
				*timer.m_Timer = 1.0 - (timer.m_CurrentTime * (1.0 / timer.m_StartTime));
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

	void Time::StartNormalizedTimer10(float& Duration)
	{
		NormalizedTimer10 newNormalizedTimer(Duration);
		m_NormalizedTimers10.push_back(newNormalizedTimer); // add to timers to tick down
	}

	void Time::StartNormalizedTimer01(float& Duration)
	{
		NormalizedTimer01 newNormalizedTimer(Duration);
		m_NormalizedTimers01.push_back(newNormalizedTimer); // add to timers to tick down
	}

	void Time::OnUpdate()
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

