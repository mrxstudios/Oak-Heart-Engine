#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <unordered_map>

class Profiler {
public:
    static Profiler& GetInstance();

    void Start(const std::string& name);
    void Stop(const std::string& name);
    void PrintResults();

private:
    struct ProfileData {
        std::chrono::high_resolution_clock::time_point startTime;
        long long duration = 0;
        int count = 0;
    };

    std::unordered_map<std::string, ProfileData> profiles;

    Profiler() = default;
};

#define PROFILE_FUNCTION Profiler::GetInstance().Start(__FUNCTION__); \
                          at_scope_exit([]{ Profiler::GetInstance().Stop(__FUNCTION__); })

#define START_PROFILE(name) Profiler::GetInstance().Start(name);
#define STOP_PROFILE(name) Profiler::GetInstance().Stop(name);

#define PROFILE_SCOPE(name) ProfileScope profileScope_##__LINE__(name);
#define START_PROFILE_COUNT(name, count) Profiler::GetInstance().Start(name, count);

template <typename Func>
class AtScopeExit {
public:
    AtScopeExit(Func&& func) : m_func(std::move(func)) {}
    ~AtScopeExit() { m_func(); }
private:
    Func m_func;
};

template <typename Func>
AtScopeExit<Func> at_scope_exit(Func&& func) {
    return AtScopeExit<Func>(std::forward<Func>(func));
}

class ProfileScope {
public:
    ProfileScope(const std::string& name) : m_name(name) {
        Profiler::GetInstance().Start(name);
    }

    ~ProfileScope() {
        Profiler::GetInstance().Stop(m_name);
    }

private:
    std::string m_name;
};
