#ifndef _CHROMA_TIME_H_
#define _CHROMA_TIME_H_

#include <chrono>
#include <thread>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <ChromaConfig.h>
#include <math/Math.h>
#include <core/Core.h>



namespace Chroma
{
	class Time
	{
	private:

		void Sleep(int milliseconds);
		static double m_MaxMSPerFrame;

		static double m_Current;
		static double m_Previous;

		static double m_Delta;
		static double m_FPS;

		static double m_Lag;

		static float m_Speed;

	public:
		static void Init();
		inline static double& GetLag() { return m_Lag; }
		inline static void DecreaseLag(double decreaseAmount) { m_Lag -= decreaseAmount; }
		inline static double& GetMSPerFrame() { return m_MaxMSPerFrame; }
		inline static double& GetDeltaTime() { return  m_Delta; }
		inline static double GetGameTime() { return m_Speed * glfwGetTime(); }
		inline static double& GetFPS() { return m_FPS; }

		inline static void SetSpeed(const float& newSpeed) { m_Speed = newSpeed; }

		static float GetLoopingTime(float const& loopDuration);
		static float GetLoopingTimeNormalized(float const& loopDuration);
		static void Process();
	};

}


#endif

#define CHROMA_GAMETIME Chroma::Time::GetGameTime()
#define CHROMA_DELTA_TIME Chroma::Time::GetDeltaTime()