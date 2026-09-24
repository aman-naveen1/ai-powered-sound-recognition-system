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

install(TARGETS sound_recognition RUNTIME DESTINATION .)
install(DIRECTORY python DESTINATION . FILES_MATCHING PATTERN "*.py")
install(FILES requirements.txt .env.example DESTINATION .)

set(CPACK_PACKAGE_NAME "sound_recognition")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_GENERATOR "ZIP")
include(CPack)
