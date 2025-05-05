import tkinter as tk
from tkinter import filedialog, messagebox
import pygame
import os

# Initialize Pygame for music playback
pygame.mixer.init()

# Create the main application window
root = tk.Tk()
root.title("Media Player")
root.geometry("400x300")
root.resizable(False, False)

# Variables
playing = False
current_volume = 1.0  # Default volume is max
audio_file = None  # Initially no audio file selected

# Play or Pause the music
def play_pause():
    global playing
    if audio_file is None:
        messagebox.showwarning("No File", "Please select an audio file first!")
        return

    if playing:
        pygame.mixer.music.pause()
        play_pause_btn.config(text="Play")
    else:
        pygame.mixer.music.load(audio_file)
        pygame.mixer.music.play(-1)  # -1 means loop forever
        play_pause_btn.config(text="Pause")
    playing = not playing

# Stop the music
def stop():
    global playing
    pygame.mixer.music.stop()
    play_pause_btn.config(text="Play")
    playing = False

# Volume control
def change_volume(val):
    global current_volume
    current_volume = float(val) / 100
    pygame.mixer.music.set_volume(current_volume)

# Select audio file
def select_file():
    global audio_file
    audio_file = filedialog.askopenfilename(filetypes=[("Audio Files", "*.mp3;*.wav;*.ogg")])
    if audio_file:
        file_name = os.path.basename(audio_file)
        file_label.config(text=f"Selected: {file_name}")
    else:
        file_label.config(text="No file selected")

# Create the UI elements
select_btn = tk.Button(root, text="Select Audio File", command=select_file, font=("Arial", 14))
select_btn.pack(pady=20)

file_label = tk.Label(root, text="No file selected", font=("Arial", 12))
file_label.pack(pady=5)

play_pause_btn = tk.Button(root, text="Play", command=play_pause, font=("Arial", 14))
play_pause_btn.pack(pady=20)

stop_btn = tk.Button(root, text="Stop", command=stop, font=("Arial", 14))
stop_btn.pack(pady=10)

volume_label = tk.Label(root, text="Volume", font=("Arial", 12))
volume_label.pack(pady=5)

volume_slider = tk.Scale(root, from_=0, to=100, orient="horizontal", command=change_volume, length=200)
volume_slider.set(100)  # Set default volume to 100%
volume_slider.pack(pady=10)

# Run the application
root.mainloop()


