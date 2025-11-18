#if ENABLE_PROFILING

#include "Profiler.h"

// --- ScopedTimer ---
FrameProfiler::ScopedTimer::ScopedTimer(const char* name)
    : name_(name), start_(Clock::now()) {}

FrameProfiler::ScopedTimer::~ScopedTimer() {
    auto end = Clock::now();
    auto dur = std::chrono::duration_cast<Microseconds>(end - start_).count();

    std::lock_guard<std::mutex> lock(mutex_);
    currentFrame_.timesUs[name_] += dur;
}

// --- Frame control ---
void FrameProfiler::BeginFrame() {
    std::lock_guard<std::mutex> lock(mutex_);
    currentFrame_.timesUs.clear();
}

void FrameProfiler::EndFrame(int maxHistoryFrames) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (maxHistoryFrames > 0) {
        history_.push_back(currentFrame_);
        if (static_cast<int>(history_.size()) > maxHistoryFrames) {
            history_.erase(history_.begin());
        }
    }
}

void FrameProfiler::PrintCurrentFrameReport() {
    std::lock_guard<std::mutex> lock(mutex_);
    std::cout << "=== Frame Profiling Report ===\n";
    for (const auto& [name, us] : currentFrame_.timesUs) {
        double ms = us / 1000.0;
        std::cout << std::fixed << std::setprecision(2)
                  << name << ": " << ms << " ms\n";
    }
}

#endif // ENABLE_PROFILING