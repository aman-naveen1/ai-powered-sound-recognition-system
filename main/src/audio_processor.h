#ifndef AUDIO_PROCESSOR_H
#define AUDIO_PROCESSOR_H

#include <string>

class AudioProcessor {
public:
    static void analyzeAudio(const std::string& filename);
};

#endif // AUDIO_PROCESSOR_H
