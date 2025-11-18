# 简单用例：
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

# CMake集成

```sh
# 主项目 CMakeLists.txt
cmake_minimum_required(VERSION 3.14)
project(MyApp)

add_subdirectory(profiler)  # 假设 profiler/ 与主 CMake 同级

add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE profiler::profiler)
```