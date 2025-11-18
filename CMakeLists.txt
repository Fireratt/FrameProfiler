# profiler/CMakeLists.txt

cmake_minimum_required(VERSION 3.14)
project(profiler LANGUAGES CXX)

# ===============
# 选项：是否启用 Profiling
# ===============
option(ENABLE_PROFILING "Enable frame profiling support" ON)

# ===============
# 源文件
# ===============
set(PROFILER_PUBLIC_HEADER include/profiler/Profiler.h)
set(PROFILER_SOURCES src/Profiler.cpp)

# ===============
# 创建库目标
# ===============
add_library(profiler STATIC)

# 设置包含目录（PUBLIC 表示使用者也能访问）
target_include_directories(profiler
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# 根据 ENABLE_PROFILING 决定是否编译 .cpp 和定义宏
if(ENABLE_PROFILING)
    target_sources(profiler PRIVATE ${PROFILER_SOURCES})
    target_compile_definitions(profiler PUBLIC ENABLE_PROFILING=1)
    message(STATUS "Profiling enabled")
else()
    target_compile_definitions(profiler PUBLIC ENABLE_PROFILING=0)
    message(STATUS "Profiling disabled (zero overhead)")
endif()

# 设置 C++ 标准
target_compile_features(profiler PUBLIC cxx_std_17)
set_target_properties(profiler PROPERTIES CXX_EXTENSIONS OFF)

# ===============
# 导出目标（便于主项目 find_package 或 add_subdirectory）
# ===============
add_library(profiler::profiler ALIAS profiler)