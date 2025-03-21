# 🎵 AI-Powered Sound Recognition System  

🔥 **An advanced C++ & Python hybrid program for real-time audio analysis and classification using AI.**  

![Sound Recognition](https://img.shields.io/badge/Audio%20Analysis-Librosa-blue)  
![C++](https://img.shields.io/badge/C++-17-blue) ![Python](https://img.shields.io/badge/Python-3.9%2B-yellow) ![GitHub](https://img.shields.io/github/stars/aman-naveen1/ai-powered-sound-recognition-system?style=social)  

🚀 **This project records, processes, and analyzes sound to extract key features (tempo, intensity, MFCCs, etc.) and classify it into categories like "Pop," "Rock," or "Classical."**  

---

## 📌 **Features**  
✅ **Record & Analyze Audio in Real-Time**  
✅ **Extract Key Sound Features** (Tempo, Spectral Centroid, Zero-Crossing Rate, MFCCs)  
✅ **Classify Music Genre & Intensity using AI**  
✅ **C++-Powered GUI (ASCII-based) with a Spectrogram**  

---

## 🔧 **Installation & Setup**  

### **1️⃣ Clone the Repository**  
```sh
git clone https://github.com/aman-naveen1/ai-powered-sound-recognition-system.git
cd ai-powered-sound-recognition-system
2️⃣ Install Dependencies
For C++ Compilation
sh
Copy
Edit
sudo apt install g++ cmake
For Python Libraries
sh
Copy
Edit
pip install librosa numpy scipy matplotlib
🎼 Usage
1️⃣ Run the C++ Program
sh
Copy
Edit
cd src
g++ main.cpp sound_recognition.cpp audio_processor.cpp utils.cpp -o sound_recognition
./sound_recognition
2️⃣ Process an Audio File (AI-Based Analysis - Python)
sh
Copy
Edit
cd python
python process_audio.py ../assets/example.wav
3️⃣ Run Tests
sh
Copy
Edit
cd tests
python -m unittest test_features.py
🛠️ Project Structure
perl
Copy
Edit
📂 ai-powered-sound-recognition-system/
├── 📂 src/                   # C++ Source files
│   ├── main.cpp              # Entry point
│   ├── sound_recognition.cpp # Core logic
│   ├── sound_recognition.h   # Header file
│   ├── audio_processor.cpp   # Audio feature extraction
│   ├── audio_processor.h     # Header file for processing
│   ├── utils.cpp             # Utility functions
│   ├── utils.h               # Header file for utilities
├── 📂 python/                # AI-based sound analysis
│   ├── process_audio.py      # Extracts sound features
│   ├── classify_sound.py     # AI model for classification
├── 📂 assets/                # Sample audio files
│   ├── example.wav           # Sample audio file for testing
├── 📂 docs/                  # Documentation
│   ├── feature_analysis.md   # Audio feature details
├── 📂 tests/                 # Unit tests
│   ├── test_audio.cpp        # Tests for C++ functions
│   ├── test_features.py      # Tests for Python feature extraction
├── .gitignore                # Ignore compiled files & logs
├── LICENSE                   # MIT License
├── README.md                 # Project setup & guide
├── Makefile / CMakeLists.txt # Build automation (optional)
🎯 Future Improvements
✔ Microphone Streaming (Real-Time Processing)
✔ Deep Learning Model for Better Genre Classification
✔ Better Audio Visualization (Waveforms & Spectrograms)

📝 License
This project is licensed under the MIT License.

🤝 Contributions & Issues
Want to improve this project? Fork, contribute, or report bugs!
📩 Open an issue or pull request on GitHub.

🚀 Star ⭐ the repo if you found this useful!
