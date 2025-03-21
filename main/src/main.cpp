#include <iostream>
#include "sound_recognition.h"

int main() {
    SoundRecognition app;
    int choice;
    std::string filePath;

    while (true) {
        app.displayMenu();
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                app.recordAudio();
                break;
            case 2:
                std::cout << "Enter audio file path: ";
                std::cin >> filePath;
                app.processAudio(filePath);
                break;
            case 3:
                app.analyzeFeatures();
                break;
            case 4:
                app.displayResults();
                break;
            case 5:
                std::cout << "Exiting...\n";
                return 0;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }
}
