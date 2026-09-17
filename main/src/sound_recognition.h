#ifndef SOUND_RECOGNITION_H
#define SOUND_RECOGNITION_H

#include <string>

class SoundRecognition {
private:
    std::string lastFile;
    double lastTempo;
    std::string lastGenre;
    std::string lastIntensity;

public:
    SoundRecognition();
    void displayMenu();
    void recordAudio();
    void processAudio(const std::string& filePath);
    void analyzeFeatures();
    void classifyAudio();
    void displayResults();
    void findOnSpotify();
};

#endif
