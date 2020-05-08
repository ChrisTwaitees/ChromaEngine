// Basic instrumentation profiler by Cherno: https://gist.github.com/TheCherno/31f135eea6ee729ab5f26a6908eb3a5e
#ifndef CHROMA_INSTRUMENTATION_H
#define CHROMA_INSTRUMENTATION_H

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>

#include <thread>


namespace Chroma
{
	struct ProfileResult
	{
		std::string Name;
		long long Start, End;
		uint32_t ThreadID;
		float GetMS()
		{
			return (float)(End - Start) / 1000.0f;
		}
	};

	struct InstrumentationSession
	{
		std::string Name;
	};

	class Instrumentor
	{
	private:
		InstrumentationSession* m_CurrentSession;
		std::ofstream m_OutputStream;
		std::vector<ProfileResult> m_ProfileResults;
		int m_ProfileCount{ 0 };
		bool m_WriteToFile{ false };
	
	public:
		Instrumentor()
			: m_CurrentSession(nullptr), m_ProfileCount(0), m_WriteToFile(false)
		{
		}

		void BeginSession(const std::string& name, const std::string& filepath) // filepath must be .json
		{
			
			m_WriteToFile = true;
			m_OutputStream.open(filepath);
			WriteHeader();
			m_CurrentSession = new InstrumentationSession{ name };
		}

		void BeginSession(const std::string& name)
		{

			m_WriteToFile = false;
			m_CurrentSession = new InstrumentationSession{ name };
		}


		void EndSession()
		{
			if (m_WriteToFile)
			{
				WriteFooter();
				m_OutputStream.close();
			}
			delete m_CurrentSession;
			m_CurrentSession = nullptr;
			m_ProfileCount = 0;
			m_ProfileResults.clear();
		}

		void WriteProfile(const ProfileResult& result)
		{
			if (m_ProfileCount++ > 0)
				m_OutputStream << ",";

			std::string name = result.Name;
			std::replace(name.begin(), name.end(), '"', '\'');

			m_OutputStream << "{";
			m_OutputStream << "\"cat\":\"function\",";
			m_OutputStream << "\"dur\":" << (result.End - result.Start) << ',';
			m_OutputStream << "\"name\":\"" << name << "\",";
			m_OutputStream << "\"ph\":\"X\",";
			m_OutputStream << "\"pid\":0,";
			m_OutputStream << "\"tid\":" << result.ThreadID << ",";
			m_OutputStream << "\"ts\":" << result.Start;
			m_OutputStream << "}";

			m_OutputStream.flush();
		}

		void WriteHeader()
		{
			m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
			m_OutputStream.flush();
		}

		void WriteFooter()
		{
			m_OutputStream << "]}";
			m_OutputStream.flush();
		}

		void AddProfileResult(const ProfileResult& profileResult)
		{
			m_ProfileCount++;
			m_ProfileResults.push_back(profileResult);
		}

		inline std::vector<ProfileResult> GetProfileResults() { return m_ProfileResults; };

		bool GetWriteToFile() { return m_WriteToFile; }

		static Instrumentor& Get()
		{
			static Instrumentor instance;
			return instance;
		}
	};

	class InstrumentationTimer
	{
	public:
		InstrumentationTimer(const char* name)
			: m_Name(name), m_Stopped(false)
		{
			m_StartTimepoint = std::chrono::high_resolution_clock::now();
		}

		~InstrumentationTimer()
		{
			if (!m_Stopped)
				Stop();
		}

		void Stop()
		{
			auto endTimepoint = std::chrono::high_resolution_clock::now();

			long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
			long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

			uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
			if (Instrumentor::Get().GetWriteToFile())
			{
				Instrumentor::Get().WriteProfile({ m_Name, start, end, threadID });
			}

			Instrumentor::Get().AddProfileResult({ m_Name, start, end, threadID });

			m_Stopped = true;
		}
	private:
		const char* m_Name;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
		bool m_Stopped;
	};

}

#if DEBUG
	#define CHROMA_PROFILE_BEGIN_RECORDED_SESSION(name, filepath) ::Chroma::Instrumentor::Get().BeginSession(name, filepath)
	#define CHROMA_PROFILE_BEGIN_SESSION(name) ::Chroma::Instrumentor::Get().BeginSession(name)
	#define CHROMA_PROFILE_END_SESSION() ::Chroma::Instrumentor::Get().EndSession()
	#define CHROMA_PROFILE_SCOPE(name) ::Chroma::InstrumentationTimer timer##__LINE__(name);
	#define CHROMA_PROFILE_FUNCTION() CHROMA_PROFILE_SCOPE(__FUNCSIG__)
#else
	#define CHROMA_PROFILE_BEGIN_RECORDED_SESSION(name, filepath) 
	#define CHROMA_PROFILE_BEGIN_SESSION(name)
	#define CHROMA_PROFILE_END_SESSION() 
	#define CHROMA_PROFILE_SCOPE(name)
	#define CHROMA_PROFILE_FUNCTION()
#endif // DEBUG
#endif //CHROMA_INSTRUMENTATION_H