#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>
#include "sound_recognition.h"

SoundRecognition::SoundRecognition()
    : lastFile(), lastTempo(0.0), lastGenre("Unknown"), lastIntensity("Unknown") {}

void SoundRecognition::setExecutablePath(const std::string& exePath) {
    if (exePath.empty()) {
        return;
    }

    std::filesystem::path path(exePath);
    if (path.is_relative()) {
        path = std::filesystem::absolute(path);
    }
    executableDir = path.parent_path().string();
}

std::string SoundRecognition::resolveProcessAudioScript() const {
    std::vector<std::filesystem::path> candidates;

    if (!executableDir.empty()) {
        std::filesystem::path exeDir(executableDir);
        candidates.push_back(exeDir / "python" / "process_audio.py");
        candidates.push_back(exeDir / ".." / "python" / "process_audio.py");
        candidates.push_back(exeDir / ".." / ".." / "python" / "process_audio.py");
    }

    std::filesystem::path cwd = std::filesystem::current_path();
    candidates.push_back(cwd / "python" / "process_audio.py");
    candidates.push_back(cwd / ".." / "python" / "process_audio.py");

    for (const auto& candidate : candidates) {
        if (std::filesystem::exists(candidate)) {
            return std::filesystem::weakly_canonical(candidate).string();
        }
    }

    return "";
}

void SoundRecognition::displayMenu() {
    std::cout << "\n===================================\n";
    std::cout << "|        SOUND RECOGNITION        |\n";
    std::cout << "===================================\n";
    std::cout << "| 1. Record Audio                |\n";
    std::cout << "| 2. Process Audio File          |\n";
    std::cout << "| 3. Analyze Features            |\n";
    std::cout << "| 4. Display Results             |\n";
    std::cout << "| 5. Exit                        |\n";
    std::cout << "===================================\n";
}

void SoundRecognition::recordAudio() {
    std::cout << "[INFO] Recording is not available in the CLI build yet.\n";
    std::cout << "[INFO] Use option 2 with a WAV/MP3/FLAC file.\n";
}

void SoundRecognition::processAudio(const std::string& filePath) {
    lastFile = filePath;
    std::cout << "[INFO] Processing: " << filePath << "\n";

    std::string scriptPath = resolveProcessAudioScript();
    if (scriptPath.empty()) {
        std::cerr << "[ERROR] Could not locate python/process_audio.py. "
                  << "Place the python folder beside the executable or run from the project root.\n";
        return;
    }

    std::string command = "python \"" + scriptPath + "\" \"" + filePath + "\"";
    int result = std::system(command.c_str());

    if (result != 0) {
        std::cerr << "[ERROR] Audio analysis failed. Check Python and Librosa installation.\n";
        return;
    }

    std::cout << "[INFO] Audio processing completed.\n";
}

void SoundRecognition::analyzeFeatures() {
    if (lastFile.empty()) {
        std::cout << "[WARN] Process an audio file first.\n";
        return;
    }

    std::cout << "[INFO] Running feature analysis...\n";
    std::string scriptPath = resolveProcessAudioScript();
    if (scriptPath.empty()) {
        std::cerr << "[ERROR] Could not locate python/process_audio.py. "
                  << "Place the python folder beside the executable or run from the project root.\n";
        return;
    }

    std::string command = "python \"" + scriptPath + "\" \"" + lastFile + "\"";
    int result = std::system(command.c_str());

    if (result != 0)
        std::cerr << "[ERROR] Feature analysis failed.\n";
}

void SoundRecognition::classifyAudio() {
    std::cout << "[INFO] Classification is based on extracted audio features.\n";
}

void SoundRecognition::displayResults() {
    std::cout << "\n========== RESULTS ==========\n";
    if (lastFile.empty()) {
        std::cout << "No audio file has been processed yet.\n";
    } else {
        std::cout << "File: " << lastFile << "\n";
        std::cout << "Use the feature-analysis output above for BPM and audio features.\n";
    }
    std::cout << "=============================\n";
}
