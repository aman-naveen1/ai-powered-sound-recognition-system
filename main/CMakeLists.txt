cmake_minimum_required(VERSION 3.16)
project(sound_recognition LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(sound_recognition
    src/main.cpp
    src/sound_recognition.cpp
    src/audio_processor.cpp
    src/utils.cpp
)

target_include_directories(sound_recognition PRIVATE src)
