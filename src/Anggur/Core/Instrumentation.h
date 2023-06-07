#pragma once

#include <stack>
#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>

#include <thread>

namespace Anggur::Instrumentation 
{
	struct Profile
	{
		uint32_t threadID;

		std::string name;

		long long beginTime;
		long long endTime;
	};

	struct Session
	{
	public:
		Session(const std::string& name, const std::string& path);

		~Session();

		void WriteProfile(const Profile& result);

	private:
		void WriteOpening();

		void WriteClosing();

	private:
		std::string name;
		std::string path;

		std::ofstream outputStream;

		int profileCount = 0;
	};

	class Watcher
	{
	private:
		std::stack<Session> sessions;

	public:
		Watcher();

		void BeginSession(const std::string& name, const std::string& filepath = "results.json");

		void EndSession();

		bool HasActiveSession();

		Session& GetActiveSession();

		static Watcher& GetInstance();
	};

	class Timer
	{
	public:
		Timer(const char* name);

		~Timer();

	private:
		const char* name;
		std::chrono::time_point<std::chrono::high_resolution_clock> beginTimepoint;
	};
}

#if 1
#define ANGGUR_INSTRUMENTATION_SESSION_BEGIN(name) Anggur::Instrumentation::Watcher::GetInstance().BeginSession(name)
#define ANGGUR_INSTRUMENTATION_PROFILE(name) Anggur::Instrumentation::Timer timer ## __LINE__ (name)
#define ANGGUR_INSTRUMENTATION_PROFILE_FUNCTION Anggur::Instrumentation::Timer timer ## __LINE__ (__FUNCTION__)
#define ANGGUR_INSTRUMENTATION_SESSION_END Anggur::Instrumentation::Watcher::GetInstance().EndSession()
#else
#define ANGGUR_INSTRUMENTATION_SESSION_BEGIN(name)
#define ANGGUR_INSTRUMENTATION_PROFILE(name)
#define ANGGUR_INSTRUMENTATION_PROFILE_FUNCTION
#define ANGGUR_INSTRUMENTATION_SESSION_END
#endif
