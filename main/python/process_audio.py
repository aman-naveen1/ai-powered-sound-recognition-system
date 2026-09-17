import sys
from pathlib import Path

import librosa
import numpy as np


def extract_features(audio_file):
    path = Path(audio_file)
    if not path.is_file():
        raise FileNotFoundError(f"Audio file not found: {audio_file}")

    y, sr = librosa.load(path, sr=None, mono=True)

    if len(y) == 0:
        raise ValueError("The audio file contains no samples.")

    tempo, _ = librosa.beat.beat_track(y=y, sr=sr)
    tempo = float(np.asarray(tempo).reshape(-1)[0])

    rms = float(np.mean(librosa.feature.rms(y=y)))
    centroid = float(np.mean(librosa.feature.spectral_centroid(y=y, sr=sr)))
    zcr = float(np.mean(librosa.feature.zero_crossing_rate(y)))

    if rms < 0.03:
        intensity = "Soft"
    elif rms < 0.10:
        intensity = "Moderate"
    else:
        intensity = "Loud"

    if tempo >= 120 and centroid >= 2500:
        genre = "Rock"
    elif tempo >= 100:
        genre = "Pop"
    elif tempo < 80 and centroid < 2200:
        genre = "Classical"
    else:
        genre = "Jazz"

    print(f"File: {path}")
    print(f"Sample rate: {sr} Hz")
    print(f"Tempo: {tempo:.2f} BPM")
    print(f"Intensity: {intensity}")
    print(f"Genre estimate: {genre}")
    print(f"Spectral centroid: {centroid:.2f} Hz")
    print(f"Zero-crossing rate: {zcr:.4f}")

    return {
        "tempo": tempo,
        "intensity": intensity,
        "genre": genre,
        "spectral_centroid": centroid,
        "zero_crossing_rate": zcr,
    }


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python process_audio.py <audio_file>")
        sys.exit(1)

    try:
        extract_features(sys.argv[1])
    except Exception as exc:
        print(f"[ERROR] {exc}", file=sys.stderr)
        sys.exit(1)
