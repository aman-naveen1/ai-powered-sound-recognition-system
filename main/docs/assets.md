# Sound Recognition System - Architecture

## Components:
- **C++ Core**: Handles UI and calls Python scripts.
- **Python AI Module**: Uses Librosa for sound analysis.
- **ASCII UI**: Provides a user-friendly terminal interface.

## Workflow:
1. User records audio (`recordAudio()`).
2. Audio is processed (`process_audio.py`).
3. AI classifies the sound (`classify_sound.py`).
4. Results are displayed (`results.txt`).
