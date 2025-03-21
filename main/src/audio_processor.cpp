#include "audio_processor.h"
#include <iostream>

void AudioProcessor::analyzeAudio(const std::string& filename) {
    std::cout << "Analyzing audio file: " << filename << "\n";
    system(("python ../python/process_audio.py " + filename).c_str());
}
