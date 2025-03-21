import unittest
import librosa
import numpy as np

from pathlib import Path

class TestAudioFeatures(unittest.TestCase):

    def setUp(self):
        """Load the test audio file before each test"""
        self.test_audio = "assets/example.wav"
        self.assertTrue(Path(self.test_audio).exists(), "Test audio file not found!")

        # Load the audio file
        self.y, self.sr = librosa.load(self.test_audio)

    def test_tempo_detection(self):
        """Check if tempo (BPM) is within a valid range"""
        tempo, _ = librosa.beat.beat_track(y=self.y, sr=self.sr)
        self.assertGreater(tempo, 40, "Tempo is too slow (below 40 BPM)")
        self.assertLess(tempo, 220, "Tempo is too fast (above 220 BPM)")

    def test_spectral_centroid(self):
        """Ensure spectral centroid values are within human hearing range"""
        centroid = librosa.feature.spectral_centroid(y=self.y, sr=self.sr)
        mean_centroid = np.mean(centroid)
        self.assertGreater(mean_centroid, 20, "Spectral centroid is too low (below human hearing range)")
        self.assertLess(mean_centroid, 20000, "Spectral centroid is too high (above human hearing range)")

    def test_zero_crossing_rate(self):
        """Ensure zero-crossing rate is within a reasonable range"""
        zcr = librosa.feature.zero_crossing_rate(self.y)
        mean_zcr = np.mean(zcr)
        self.assertGreater(mean_zcr, 0.001, "Zero crossing rate is too low (may indicate silence)")
        self.assertLess(mean_zcr, 0.5, "Zero crossing rate is too high (may indicate excessive noise)")

    def test_mfcc_shape(self):
        """Check if MFCC feature extraction returns the correct shape"""
        mfccs = librosa.feature.mfcc(y=self.y, sr=self.sr, n_mfcc=13)
        self.assertEqual(mfccs.shape[0], 13, "MFCC should have 13 coefficients")

if __name__ == '__main__':
    unittest.main()
