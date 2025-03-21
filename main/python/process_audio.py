import librosa
import sys

def extract_features(audio_file):
    y, sr = librosa.load(audio_file)
    tempo, _ = librosa.beat.beat_track(y=y, sr=sr)
    print(f"Extracted Tempo: {tempo:.2f} BPM")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python process_audio.py <audio_file>")
    else:
        extract_features(sys.argv[1])
