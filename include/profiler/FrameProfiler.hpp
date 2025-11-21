#pragma once

// === 用户可通过 -DENABLE_PROFILING=0/1 控制，否则默认关闭 ===
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

namespace profiler {

class ScopedTimer {
public:
    explicit ScopedTimer(const char* name);
    ~ScopedTimer();

private:
    const char* name_;
    std::chrono::high_resolution_clock::time_point start_;
    
};

void BeginFrame();
void EndFrame(int maxHistoryFrames = 60);
void PrintCurrentFrameReport();

} // namespace profiler

#define PROFILE_SCOPE(name) ::profiler::ScopedTimer _profiler_scope_##__LINE__(name)
#define PROFILE_BEGIN_FRAME() ::profiler::BeginFrame()
#define PROFILE_END_FRAME(n) ::profiler::EndFrame(n)
#define PROFILE_PRINT_FRAME() ::profiler::PrintCurrentFrameReport()

#else

// --- 空实现：所有函数 inline empty ---
namespace profiler {
struct ScopedTimer {
    explicit ScopedTimer(const char*) noexcept {}
    ~ScopedTimer() = default;
};
inline void BeginFrame() noexcept {}
inline void EndFrame(int = 60) noexcept {}
inline void PrintCurrentFrameReport() noexcept {}
} // namespace profiler

#define PROFILE_SCOPE(name) (void)0
#define PROFILE_BEGIN_FRAME() (void)0
#define PROFILE_END_FRAME(n) (void)0
#define PROFILE_PRINT_FRAME() (void)0

#endif