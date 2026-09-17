#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <thread>
#include "sound_recognition.h"

SoundRecognition::SoundRecognition()
    : lastFile(), lastTempo(0.0), lastGenre("Unknown"), lastIntensity("Unknown") {}

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

    std::string command = "python python/process_audio.py \"" + filePath + "\"";
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
    std::string command = "python python/process_audio.py \"" + lastFile + "\"";
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
