简单用例：
```cpp
#define ENABLE_PROFILING
void StageB() {
    PROFILE_SCOPE("B");
    // ... work
}

void StageA() {
    PROFILE_SCOPE("A");
    StageB();
}

int main() {
    for (int frame = 0; frame < 100; ++frame) {
        PROFILE_BEGIN_FRAME();

        StageA();

        PROFILE_END_FRAME(30);

        if (frame % 10 == 0) {
            PROFILE_PRINT_FRAME();
        }
    }
}
```
