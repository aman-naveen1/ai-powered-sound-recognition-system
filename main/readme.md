# 🎵 AI-Powered Sound Recognition System

A C++17 command-line sound analysis project with Python + Librosa and optional real song recognition through AudD, with Spotify metadata and album artwork URLs.

## What it does

- Processes WAV, MP3, FLAC and other formats supported by Librosa.
- Extracts **BPM**, RMS-based **intensity**, **spectral centroid**, and **zero-crossing rate**.
- Produces a lightweight rule-based genre estimate.
- Can identify a commercially released song from a short audio clip using AudD.
- When a Spotify match is available, returns the **track, artist, album, Spotify URL, and original Spotify album-art URL**.
- Keeps secrets outside the repository using environment variables.
- Includes a CMake build configuration.

## Recognition pipeline

```text
Audio file
   │
   ├──► Librosa ──► BPM / intensity / spectral features
   │
   └──► AudD ──► song title + artist
                    │
                    └──► Spotify metadata ──► album + artwork URL + Spotify link
```

AudD's standard recognition endpoint accepts a local audio file and can return Spotify metadata when requested. citeturn0search0turn1search1

Spotify's Web API exposes album artwork URLs as part of album/track metadata. Spotify requires visual content to remain in its original form and metadata/artwork to include attribution and a link back to Spotify. citeturn1search2turn1search3

## Project structure

```text
main/
├── CMakeLists.txt
├── requirements.txt
├── .env.example
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
│   ├── recognize_song.py
│   ├── spotify_lookup.py
│   └── classify_sounds.py
└── assets/
    └── crowd-cheering.wav
```

## Requirements

- C++17 compiler: GCC, Clang, or MSVC
- CMake 3.16+
- Python 3.9+
- Python packages from `requirements.txt`
- An AudD API token for audio recognition
- Spotify Web API Client ID and Client Secret for direct Spotify searches

## Install Python dependencies

```bash
cd main
python -m pip install -r requirements.txt
```

## Configure API credentials

Copy `.env.example` to `.env`, then fill in your own credentials.

For shells that do not automatically load `.env`, export the variables yourself.

PowerShell:

```powershell
$env:AUDD_API_TOKEN="your_audd_token"
$env:SPOTIFY_CLIENT_ID="your_spotify_client_id"
$env:SPOTIFY_CLIENT_SECRET="your_spotify_client_secret"
```

Do **not** commit `.env`. It is ignored by Git.

Spotify's Client Credentials flow is intended for server-to-server access to endpoints that do not require user authorization. Spotify also specifically recommends PKCE rather than embedding a client secret in desktop/mobile apps where the secret cannot be kept safe. citeturn0search4turn0search8

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

## Deploy as a Windows EXE package

Create an install folder (contains the EXE + Python scripts):

```powershell
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
cmake --install build --config Release --prefix build\deploy
```

Then generate a distributable ZIP package:

```powershell
cd build
cpack -C Release
```

The generated ZIP includes:
- `sound_recognition.exe`
- `python/process_audio.py` and related Python modules
- `.env.example`
- `requirements.txt`

Before running the deployed EXE on another machine, install Python 3.9+ and run:

```powershell
python -m pip install -r requirements.txt
```

## Identify a song from an audio file

The recognition module can be tested independently:

```bash
python python/recognize_song.py assets/crowd-cheering.wav
```

For an actual music clip, the output can include:

```text
========== SONG MATCH ==========
Title: ...
Artist: ...
Album: ...
Spotify: https://open.spotify.com/...
Album cover: https://i.scdn.co/...
===============================
```

The cover is exposed as Spotify's original artwork URL rather than copied into the repository. This follows Spotify's artwork/content requirements. citeturn1search2turn1search3

## Direct Spotify lookup

If you already know the song title and artist:

```bash
python python/spotify_lookup.py "Blinding Lights The Weeknd"
```

This uses Spotify's search endpoint to retrieve the matching track and its album artwork URL. citeturn0search1

## Important limitations

- Song recognition requires an AudD API token and a network connection.
- Recognition returns no match when the service cannot identify the clip.
- The current genre classifier is a simple feature-based demonstration, not a trained ML model.
- The current C++ CLI does not record from a microphone yet.
- Spotify credentials should not be embedded in a distributed desktop executable. For a production desktop app, use a safer architecture/authorization flow rather than shipping a client secret. citeturn0search8

## License

Apache License 2.0.
