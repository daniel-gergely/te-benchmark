cmake_minimum_required (VERSION 3.15...3.20)

project(benchmark VERSION 1.0.0)

add_subdirectory(tracktion_engine/modules/juce)
add_subdirectory(tracktion_engine)

juce_add_console_app(benchmark)
target_sources(benchmark PUBLIC main.cpp)
target_link_libraries(benchmark PUBLIC
    tracktion::tracktion_engine
    )

target_compile_definitions(benchmark PUBLIC TRACKTION_LOG_ENABLED=0)