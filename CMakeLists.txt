# profiler/CMakeLists.txt

cmake_minimum_required(VERSION 3.14)
project(profiler LANGUAGES CXX)

# ===============
# 选项：是否启用 Profiling（仅用于设置宏值）
# ===============
option(ENABLE_PROFILING "Enable frame profiling support" OFF)

# ===============
# 源文件（始终包含）
# ===============
set(PROFILER_PUBLIC_HEADER include/profiler/FrameProfiler.hpp)
set(PROFILER_SOURCES src/FrameProfiler.cpp)

# ===============
# 创建静态库
# ===============
add_library(profiler STATIC
    ${PROFILER_SOURCES}
)

# 设置包含目录
target_include_directories(profiler
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# 传递 ENABLE_PROFILING 宏（0 或 1）
if(ENABLE_PROFILING)
    target_compile_definitions(profiler PUBLIC ENABLE_PROFILING=1)
    message(STATUS "Profiling enabled (ENABLE_PROFILING=1)")
else()
    target_compile_definitions(profiler PUBLIC ENABLE_PROFILING=0)
    message(STATUS "Profiling disabled (ENABLE_PROFILING=0, zero overhead)")
endif()

# C++ 标准
target_compile_features(profiler PUBLIC cxx_std_17)
set_target_properties(profiler PROPERTIES CXX_EXTENSIONS OFF)

# 别名便于使用
add_library(profiler::profiler ALIAS profiler)