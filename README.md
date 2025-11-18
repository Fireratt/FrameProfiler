简单用例：
```cpp
void StageB() {
    FrameProfiler::ScopedTimer timer("B");
    // ... 工作
}

void StageC() {
    FrameProfiler::ScopedTimer timer("C");
    // ... 工作
}

void StageA() {
    FrameProfiler::ScopedTimer timer("A"); // A 包含 B+C
    StageB();
    StageC();
}

int main() {
    for (int frame = 0; frame < 100; ++frame) {
        FrameProfiler::BeginFrame(); // ← 每帧开始重置

        StageA(); // 执行你的逻辑

        FrameProfiler::EndFrame(30); // 保留最近 30 帧历史（可选）

        // 可选：打印当前帧
        if (frame % 10 == 0) {
            FrameProfiler::PrintCurrentFrameReport();
        }
    }
}
```
