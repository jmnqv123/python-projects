import speech_recognition as sr

# Listen and convert voice to text
def listen():
    recognizer = sr.Recognizer()
    with sr.Microphone() as source:
        print("🎤 Please say which bill you want to pay (e.g., electricity, water, internet)...")
        audio = recognizer.listen(source)

    try:
        text = recognizer.recognize_google(audio)
        print(f"✅ You said: {text}")
        return text.lower()
    except sr.UnknownValueError:
        print("❌ Sorry, I could not understand the audio.")
    except sr.RequestError:
        print("❌ Could not connect to Google Speech Recognition.")
    return ""

# Match voice command and simulate payment
def pay_bill(command):
    if "electricity" in command:
        print("🔌 Paying electricity bill...")
    elif "water" in command:
        print("🚿 Paying water bill...")
    elif "internet" in command:
        print("🌐 Paying internet bill...")
    else:
        print("❌ Bill type not recognized.")

# Main flow
if __name__ == "__main__":
    voice_input = listen()
    if voice_input:
        pay_bill(voice_input)
