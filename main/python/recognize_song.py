import os
import sys
import requests

AUDD_URL = "https://api.audd.io/"


def recognize_song(audio_file):
    token = os.getenv("AUDD_API_TOKEN")
    if not token:
        raise RuntimeError("Set AUDD_API_TOKEN to enable song recognition.")

    with open(audio_file, "rb") as audio:
        response = requests.post(
            AUDD_URL,
            data={"api_token": token, "return": "spotify", "market": "IN"},
            files={"file": audio},
            timeout=45,
        )

    response.raise_for_status()
    payload = response.json()

    if payload.get("status") != "success":
        raise RuntimeError(payload.get("error", {}).get("error_message", "Recognition failed."))

    result = payload.get("result")
    if not result:
        return None

    spotify = result.get("spotify") or {}
    album = spotify.get("album") or {}
    images = album.get("images") or []
    external_urls = spotify.get("external_urls") or {}

    return {
        "title": result.get("title") or spotify.get("name") or "Unknown",
        "artist": result.get("artist") or "Unknown",
        "album": result.get("album") or album.get("name") or "Unknown",
        "spotify_url": external_urls.get("spotify", ""),
        "cover_url": images[0].get("url", "") if images else "",
        "timecode": result.get("timecode", ""),
    }


def print_result(result):
    if not result:
        print("Recognition: No matching song found.")
        return

    print("\n========== SONG MATCH ========== ")
    print(f"Title: {result['title']}")
    print(f"Artist: {result['artist']}")
    print(f"Album: {result['album']}")
    print(f"Spotify: {result['spotify_url'] or 'Not available'}")
    print(f"Album cover: {result['cover_url'] or 'Not available'}")
    if result["timecode"]:
        print(f"Matched at: {result['timecode']}")
    print("===============================\n")


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python recognize_song.py <audio_file>")
        sys.exit(1)

    try:
        print_result(recognize_song(sys.argv[1]))
    except FileNotFoundError:
        print(f"[ERROR] Audio file not found: {sys.argv[1]}")
        sys.exit(1)
    except requests.RequestException as exc:
        print(f"[ERROR] Recognition service request failed: {exc}")
        sys.exit(1)
    except Exception as exc:
        print(f"[ERROR] {exc}")
        sys.exit(1)
