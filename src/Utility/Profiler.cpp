#include "Profiler.h"

Profiler& Profiler::GetInstance() {
    static Profiler instance;
    return instance;
}

void Profiler::Start(const std::string& name) {
    profiles[name].startTime = std::chrono::high_resolution_clock::now();
}

void Profiler::Stop(const std::string& name) {
    auto endTime = std::chrono::high_resolution_clock::now();
    auto& profile = profiles[name];
    profile.duration += std::chrono::duration_cast<std::chrono::microseconds>(endTime - profile.startTime).count();
    profile.count++;
    PrintResults();
}

void Profiler::PrintResults() {
    for (const auto& profile : profiles) {
        const std::string& name = profile.first;
        const ProfileData& data = profile.second;

        if (data.count > 0) {
            std::cout << "Profile '" << name << "': ";
            std::cout << "Total Duration: " << data.duration << " us";
            std::cout << " | Average Duration: " << (data.duration / data.count) << " us" << std::endl;
        }
    }
}
