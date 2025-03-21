#ifndef SOUND_RECOGNITION_H
#define SOUND_RECOGNITION_H

#include <string>

class SoundRecognition {
public:
    SoundRecognition();
    void displayMenu();        // Shows the ASCII menu
    void recordAudio();        // Records audio using system tools
    void processAudio();       // Extracts sound features
    void classifyAudio();      // Uses AI to classify the sound
    void displayResults();     // Displays results in ASCII format
};

#endif // SOUND_RECOGNITION_H
