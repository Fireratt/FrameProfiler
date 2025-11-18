#pragma once

// ==============================
// 开关配置（可外部定义）
// ==============================
#ifndef ENABLE_PROFILING
    #define ENABLE_PROFILING 0
#endif

#if ENABLE_PROFILING

#include <chrono>
#include <unordered_map>
#include <string>
#include <vector>
#include <mutex>
#include <iostream>
#include <iomanip>

class FrameProfiler {
public:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = Clock::time_point;
    using Microseconds = std::chrono::microseconds;

    class ScopedTimer {
    public:
        explicit ScopedTimer(const char* name);
        ~ScopedTimer();

    private:
        const char* name_;
        TimePoint start_;
    };

    static void BeginFrame();
    static void EndFrame(int maxHistoryFrames = 60);
    static void PrintCurrentFrameReport();

private:
    struct FrameData {
        std::unordered_map<std::string, long long> timesUs;
    };

    static inline std::mutex mutex_;
    static inline FrameData currentFrame_;
    static inline std::vector<FrameData> history_;
};

// 用户宏（启用时）
#define PROFILE_SCOPE(name) ::FrameProfiler::ScopedTimer _profiler_scope_##__LINE__(name)
#define PROFILE_BEGIN_FRAME() ::FrameProfiler::BeginFrame()
#define PROFILE_END_FRAME(maxHist) ::FrameProfiler::EndFrame(maxHist)
#define PROFILE_PRINT_FRAME() ::FrameProfiler::PrintCurrentFrameReport()

#else

// 空实现（禁用时）
struct DummyProfiler {
    explicit DummyProfiler(const char*) {}
    ~DummyProfiler() = default;
};
// 用户宏，用于条件编译； 在禁用时不展开 减少运行时开销
#define PROFILE_SCOPE(name) (void)0
#define PROFILE_BEGIN_FRAME() (void)0
#define PROFILE_END_FRAME(maxHist) (void)0
#define PROFILE_PRINT_FRAME() (void)0

#endif