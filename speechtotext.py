import speech_recognition as sr

# Initialize recognizer class (for recognizing the speech)
recognizer = sr.Recognizer()

# Use the microphone as the audio source
with sr.Microphone() as source:
    print("Please say something...")
    # Adjust the recognizer sensitivity to ambient noise
    recognizer.adjust_for_ambient_noise(source)
    
    # Listen for the first phrase
    audio = recognizer.listen(source)
    print("Recognizing...")

    try:
        # Use Google's speech recognition
        text = recognizer.recognize_google(audio)
        print("You said: ", text)
    
    except sr.UnknownValueError:
        print("Sorry, I could not understand the audio")
    
    except sr.RequestError as e:
        print("Could not request results from Google Speech Recognition service; {0}".format(e))

