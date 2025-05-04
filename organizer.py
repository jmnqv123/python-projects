import os
import shutil
import schedule
import time
import requests

# 🔧 Change this to the folder you want to organize
WATCHED_FOLDER = r"C:\Users\YourName\Downloads"

# Folder categories
FILE_TYPES = {
    "Images": [".png", ".jpg", ".jpeg", ".gif"],
    "Docs": [".pdf", ".docx", ".txt"],
    "Zips": [".zip", ".rar"],
    "Others": []
}

# 🧹 Organize files into folders
def organize_files():
    print("🔄 Organizing files...")
    for file in os.listdir(WATCHED_FOLDER):
        full_path = os.path.join(WATCHED_FOLDER, file)
        if os.path.isfile(full_path):
            moved = False
            for folder, extensions in FILE_TYPES.items():
                if any(file.lower().endswith(ext) for ext in extensions):
                    target_folder = os.path.join(WATCHED_FOLDER, folder)
                    os.makedirs(target_folder, exist_ok=True)
                    shutil.move(full_path, os.path.join(target_folder, file))
                    moved = True
                    break
            if not moved:
                target_folder = os.path.join(WATCHED_FOLDER, "Others")
                os.makedirs(target_folder, exist_ok=True)
                shutil.move(full_path, os.path.join(target_folder, file))
    print("✅ Done!")
    send_alert("📁 Files organized successfully!")

# 🔔 Optional: Slack or Discord webhook alert
def send_alert(message):
    webhook_url = "https://your.webhook.url"  # Replace with real webhook
    try:
        requests.post(webhook_url, json={"content": message})
    except:
        pass  # Ignore error if webhook isn't set

# 🔁 Run every 10 minutes
schedule.every(10).minutes.do(organize_files)

print("🚀 File organizer started. Press Ctrl+C to stop.")
while True:
    schedule.run_pending()
    time.sleep(1)
