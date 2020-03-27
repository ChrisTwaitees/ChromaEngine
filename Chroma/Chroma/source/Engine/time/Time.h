#ifndef _CHROMA_TIME_H_
#define _CHROMA_TIME_H_

//common
#include <common/PrecompiledHeader.h>

#include <ChromaConfig.h>
#include <math/Math.h>
#include <core/Core.h>

struct NormalizedTimer
{
	float m_StartTime{ 0.0f };
	float m_CurrentTime{ 0.0f };
	float* m_Timer;
	//std::reference_wrapper<float> m_Timer;

	bool operator <(const NormalizedTimer& rhs) const
	{
		return rhs.m_StartTime > m_StartTime;
	}

	NormalizedTimer();
	NormalizedTimer(float& Duration)
	{
		m_StartTime = Duration;
		m_CurrentTime = Duration;
		Duration = 1.0f;
		m_Timer = &Duration;
	}
};


namespace Chroma
{
	class Time
	{
	public:
		static void Init();

		// get set
		inline static double& GetLag() { return m_Lag; }
		inline static void DecreaseLag(double decreaseAmount) { m_Lag -= decreaseAmount; }
		inline static double& GetMSPerFrame() { return m_MaxFrameTime; }
		inline static double& GetDeltaTime() { return  m_Delta; }
		inline static double GetGameTime() { return m_Speed * glfwGetTime(); }
		inline static double& GetFPS() { return m_FPS; }

		static float GetLoopingTime(float const& loopDuration);
		static float GetLoopingTimeNormalized(float const& loopDuration);

		inline static void SetSpeed(const float& newSpeed) { m_Speed = newSpeed; }

		// timers
		inline static void StartTimer(float& Duration) { m_Timers.insert(Duration); };
		static void StartNormalizedTimer(float& Duration);

		static void Update();

	private:

		static void Sleep(unsigned int milliseconds);
		static double m_MaxFrameTime;

		static double m_Current;
		static double m_Previous;

		static double m_Delta;
		static double m_FPS;

		static double m_Lag;

		static float m_Speed;

		static std::set<std::reference_wrapper<float>> m_Timers;
		static std::vector<NormalizedTimer> m_NormalizedTimers;

		// functions
		static void ProcessTimers();
	};

}

#define GAMETIME Chroma::Time::GetGameTime()
#define DELTATIME Chroma::Time::GetDeltaTime()

#endif

