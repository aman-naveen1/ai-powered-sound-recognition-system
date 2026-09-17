# 🎵 AI-Powered Sound Recognition System

A C++17 command-line sound analysis project with Python + Librosa for real audio feature extraction.

## What it does

- Processes WAV, MP3, FLAC and other formats supported by Librosa/audioread/soundfile.
- Extracts **BPM**, RMS-based **intensity**, **spectral centroid**, and **zero-crossing rate**.
- Produces a simple rule-based genre estimate from the extracted features.
- Keeps the application logic in C++ and uses Python for audio analysis.
- Includes a CMake build configuration.

> Note: the current CLI build does not record from a microphone. The recording menu item is intentionally marked as unavailable rather than pretending to record audio.

## Project structure

```text
main/
├── CMakeLists.txt
├── requirements.txt
├── readme.md
├── src/
│   ├── main.cpp
│   ├── sound_recognition.cpp
│   ├── sound_recognition.h
│   ├── audio_processor.cpp
│   ├── audio_processor.h
│   ├── utils.cpp
│   └── utils.h
├── python/
│   ├── process_audio.py
│   └── classify_sounds.py
└── assets/
    └── crowd-cheering.wav
```

## Requirements

- C++17 compiler: GCC, Clang, or MSVC
- CMake 3.16+
- Python 3.9+
- Python packages from `requirements.txt`

### Install Python dependencies

```bash
cd main
python -m pip install -r requirements.txt
```

## Build with CMake

From the `main` directory:

```bash
cmake -S . -B build
cmake --build build
```

Run on Linux/macOS:

```bash
./build/sound_recognition
```

On Windows:

```powershell
.\build\Debug\sound_recognition.exe
```

## Analyze an audio file

You can also test the Python analyzer directly:

```bash
python python/process_audio.py assets/crowd-cheering.wav
```

Then use option **2** in the C++ application and enter the same file path.

## Important implementation detail

The C++ program now calls the Python analyzer instead of returning fake BPM/genre values. Genre is a lightweight feature-based estimate, not a trained machine-learning model, so it should be treated as a demonstration rather than a production music classifier.

## License

Apache License 2.0.
