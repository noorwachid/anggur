#include "Anggur/Core/Instrumentation.h"
#include <iostream>

namespace Anggur::Instrumentation {
	Watcher::Watcher() {
	}

	void Watcher::beginSession(const std::string& name, const std::string& filepath) {
		sessions.emplace(name, filepath);
	}

	void Watcher::endSession() {
		if (!sessions.empty())
			sessions.pop();
	}

	Session::Session(const std::string& name, const std::string& path) : name{name}, path{path} {
		outputStream.open(path);
		writeOpening();
	}

	Session::~Session() {
		writeClosing();
		outputStream.close();
	}

	void Session::writeProfile(const Profile& result) {
		if (profileCount > 0)
			outputStream << ",";

		++profileCount;

		std::string name = result.name;
		std::replace(name.begin(), name.end(), '"', '\"');

		outputStream << "{";
		outputStream << "\"cat\":\"function\",";
		outputStream << "\"dur\":" << (result.endTime - result.beginTime) << ',';
		outputStream << "\"name\":\"" << name << "\",";
		outputStream << "\"ph\":\"X\",";
		outputStream << "\"pid\":0,";
		outputStream << "\"tid\":" << result.threadID << ",";
		outputStream << "\"ts\":" << result.beginTime;
		outputStream << "}";

		outputStream.flush();
	}

	void Session::writeOpening() {
		outputStream << "{\"otherData\": {},\"traceEvents\":[";
		outputStream.flush();
	}

	void Session::writeClosing() {
		outputStream << "]}";
		outputStream.flush();
	}

	Watcher& Watcher::getInstance() {
		static Watcher instance;
		return instance;
	}

	Timer::Timer(const char* name) : name(name) {
		beginTimepoint = std::chrono::high_resolution_clock::now();
	}

	Timer::~Timer() {
		auto endTimepoint = std::chrono::high_resolution_clock::now();

		std::hash<std::thread::id> hash;

		Profile profile;
		profile.name = name;
		profile.threadID = hash(std::this_thread::get_id());
		profile.beginTime =
			std::chrono::time_point_cast<std::chrono::microseconds>(beginTimepoint).time_since_epoch().count();
		profile.endTime =
			std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		Watcher& watcher = Watcher::getInstance();

		if (watcher.hasActiveSession())
			watcher.getActiveSession().writeProfile(profile);
	}

	bool Watcher::hasActiveSession() {
		return !sessions.empty();
	}

	Session& Watcher::getActiveSession() {
		return sessions.top();
	}
}
