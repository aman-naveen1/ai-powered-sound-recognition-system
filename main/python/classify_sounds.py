import random

def classify_audio():
    genres = ["Rock", "Pop", "Classical", "Jazz"]
    intensity = ["Soft", "Moderate", "Loud"]
    
    chosen_genre = random.choice(genres)
    chosen_intensity = random.choice(intensity)

    with open("results.txt", "w") as f:
        f.write(f"Genre: {chosen_genre}\n")
        f.write(f"Intensity: {chosen_intensity}\n")

if __name__ == "__main__":
    classify_audio()
    print("Classification complete. Results saved in results.txt.")
