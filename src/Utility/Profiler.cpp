#include "Profiler.h"

Profiler& Profiler::GetInstance() {
    static Profiler instance;
    return instance;
}

void Profiler::Start(const std::string& name,int updateRateInMilliseconds=1000) {
    if (profiles[name].init) {
        profiles[name].startTime = std::chrono::high_resolution_clock::now();
        profiles[name].updateRateInMilliseconds = updateRateInMilliseconds;
        profiles[name].init = false;
    }
}

void Profiler::Stop(const std::string& name) {
    auto endTime = std::chrono::high_resolution_clock::now();
    auto& profile = profiles[name];
    double elapsed = std::chrono::duration_cast<std::chrono::microseconds>(endTime - profile.startTime).count();
    if (elapsed > profile.updateRateInMilliseconds * 1000) {
        profile.averageElapsed = elapsed / profile.count;

        profile.init = true;
        profile.count = 1;
    }
    else {
        profile.count++;
    }
}

long long Profiler::GetElapsedTime(const std::string& name) {
    return profiles[name].averageElapsed;
}
