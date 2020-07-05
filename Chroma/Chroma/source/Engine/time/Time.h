#ifndef _CHROMA_TIME_H_
#define _CHROMA_TIME_H_

//common
#include <common/PrecompiledHeader.h>

#include <ChromaConfig.h>
#include <math/Math.h>

struct NormalizedTimer10
{
	float m_StartTime{ 0.0f };
	float m_CurrentTime{ 0.0f };
	float* m_Timer;

	bool operator <(const NormalizedTimer10& rhs) const
	{
		return rhs.m_StartTime > m_StartTime;
	}

	NormalizedTimer10(float& Duration)
	{
		m_StartTime = Duration;
		m_CurrentTime = Duration;
		Duration = 1.0f;
		m_Timer = &Duration;
	}
};

struct NormalizedTimer01
{
	float m_StartTime{ 0.0f };
	float m_CurrentTime{ 0.0f };
	float* m_Timer;

	bool operator <(const NormalizedTimer01& rhs) const
	{
		return rhs.m_StartTime > m_StartTime;
	}

	NormalizedTimer01(float& Duration)
	{
		m_StartTime = Duration;
		m_CurrentTime = Duration;
		Duration = 0.0f;
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
		static void StartNormalizedTimer10(float& Duration);
		static void StartNormalizedTimer01(float& Duration);

		static void OnUpdate();

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
		static std::vector<NormalizedTimer10> m_NormalizedTimers10;
		static std::vector<NormalizedTimer01> m_NormalizedTimers01;

		// functions
		static void ProcessTimers();
	};

}

#define GAMETIME Chroma::Time::GetGameTime()
#define DELTATIME Chroma::Time::GetDeltaTime()

#endif

