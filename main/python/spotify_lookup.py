import base64
import os
import sys

import requests

TOKEN_URL = "https://accounts.spotify.com/api/token"
SEARCH_URL = "https://api.spotify.com/v1/search"


def get_token():
    client_id = os.getenv("SPOTIFY_CLIENT_ID")
    client_secret = os.getenv("SPOTIFY_CLIENT_SECRET")
    if not client_id or not client_secret:
        raise RuntimeError(
            "Set SPOTIFY_CLIENT_ID and SPOTIFY_CLIENT_SECRET environment variables."
        )

    credentials = base64.b64encode(
        f"{client_id}:{client_secret}".encode("utf-8")
    ).decode("ascii")
    response = requests.post(
        TOKEN_URL,
        headers={"Authorization": f"Basic {credentials}"},
        data={"grant_type": "client_credentials"},
        timeout=15,
    )
    response.raise_for_status()
    return response.json()["access_token"]


def search_track(query):
    token = get_token()
    response = requests.get(
        SEARCH_URL,
        headers={"Authorization": f"Bearer {token}"},
        params={"q": query, "type": "track", "market": "IN", "limit": 1},
        timeout=15,
    )
    response.raise_for_status()
    items = response.json().get("tracks", {}).get("items", [])
    if not items:
        return None

    track = items[0]
    album = track.get("album", {})
    images = album.get("images", [])
    return {
        "track": track.get("name", "Unknown"),
        "artist": ", ".join(a.get("name", "") for a in track.get("artists", [])),
        "album": album.get("name", "Unknown"),
        "spotify_url": track.get("external_urls", {}).get("spotify", ""),
        "cover_url": images[0].get("url", "") if images else "",
    }


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python spotify_lookup.py \"song name artist\"")
        sys.exit(2)

    try:
        result = search_track(" ".join(sys.argv[1:]))
        if not result:
            print("No Spotify track found.")
            sys.exit(1)

        print(f"Track: {result['track']}")
        print(f"Artist: {result['artist']}")
        print(f"Album: {result['album']}")
        print(f"Spotify: {result['spotify_url']}")
        print(f"Cover URL: {result['cover_url']}")
    except requests.RequestException as exc:
        print(f"Spotify request failed: {exc}")
        sys.exit(1)
    except Exception as exc:
        print(f"Spotify lookup failed: {exc}")
        sys.exit(1)
