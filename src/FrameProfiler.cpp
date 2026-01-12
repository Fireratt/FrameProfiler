#include "profiler/FrameProfiler.hpp"

#if ENABLE_PROFILING

#include <mutex>
#include <map>
#include <vector>

namespace profiler {

using Clock = std::chrono::high_resolution_clock;
using Microseconds = std::chrono::microseconds;
// 新增：记录帧开始时间
static std::chrono::high_resolution_clock::time_point g_frameStart;

struct FrameData {
    std::map<std::string, long long> timesUs;
    std::vector<std::string> insertionOrder; // 记录首次出现的顺序
};
// 新增：记录帧开始时间
static std::chrono::high_resolution_clock::time_point g_frameStart;

static std::mutex g_mutex;
static FrameData g_currentFrame;
static std::vector<FrameData> g_history;

ScopedTimer::ScopedTimer(const char* name)
    : name_(name), start_(Clock::now()) {}

ScopedTimer::~ScopedTimer() {
    auto end = Clock::now();
    auto dur = std::chrono::duration_cast<Microseconds>(end - start_).count();

    std::lock_guard<std::mutex> lock(g_mutex);
    
    // 检查是否已存在
    bool existed = (g_currentFrame.timesUs.find(name_) != g_currentFrame.timesUs.end());
    g_currentFrame.timesUs[name_] += dur;

    // 如果是第一次出现，记录到 insertionOrder
    if (!existed) {
        g_currentFrame.insertionOrder.push_back(name_);
    }
}// @TODO: 添加对当前帧的总时间计时
void BeginFrame() {
    std::lock_guard<std::mutex> lock(g_mutex);
    g_currentFrame.timesUs.clear();
    g_currentFrame.insertionOrder.clear(); // 清空顺序记录
    
    // 记录帧开始时间
    g_frameStart = std::chrono::high_resolution_clock::now();

}

void EndFrame(int maxHistoryFrames) {
    std::lock_guard<std::mutex> lock(g_mutex);
    // 计算总帧时间（微秒）
    auto frameEnd = std::chrono::high_resolution_clock::now();
    auto totalUs = std::chrono::duration_cast<std::chrono::microseconds>(
        frameEnd - g_frameStart
    ).count();

    // 将总时间作为特殊项插入（放在最前面或最后面，这里放最后）
    const char* totalName = "FRAME_TOTAL";
    g_currentFrame.timesUs[totalName] = totalUs;
    g_currentFrame.insertionOrder.push_back(totalName);  // 确保它出现在报告中

    if (maxHistoryFrames > 0) {
        g_history.push_back(g_currentFrame);
        if (static_cast<int>(g_history.size()) > maxHistoryFrames) {
            g_history.erase(g_history.begin());
        }
    }
}

void PrintCurrentFrameReport() {
    std::lock_guard<std::mutex> lock(g_mutex);
    std::cout << "=== Frame Profiling Report (Insertion Order) ===\n";
    for (const auto& name : g_currentFrame.insertionOrder) {
        auto it = g_currentFrame.timesUs.find(name);
        if (it != g_currentFrame.timesUs.end()) {
            double ms = it->second / 1000.0;
            std::cout << std::fixed << std::setprecision(2)
                      << name << ": " << ms << " ms\n";
        }
    }
}


} // namespace profiler

#endif // ENABLE_PROFILING