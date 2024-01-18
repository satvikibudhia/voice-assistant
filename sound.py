import serial
from gtts import gTTS
import io

# Open serial port
ser = serial.Serial('COM10', 115200)  # replace 'COM3' with your port

def convert_text_to_speech_and_send(text):
    # Text-to-Speech conversion
    tts = gTTS(text=text, lang='en')

    # Save the generated MP3 file to an in-memory stream
    mp3_data = io.BytesIO()
    tts.save(mp3_data)

    # Send the MP3 file size to ESP32
    file_size = mp3_data.getbuffer().nbytes
    ser.write(str(file_size).encode('utf-8'))
    ser.flush()
    
    # Wait for ESP32 to process the size information
    while ser.in_waiting == 0:
        pass

    # Send the MP3 file data in chunks
    mp3_data.seek(0)
    chunk_size = 256
    while True:
        chunk = mp3_data.read(chunk_size)
        if not chunk:
            break
        ser.write(chunk)
        ser.flush()

    print("MP3 file sent successfully")

# Example usage
convert_text_to_speech_and_send("Hello, this is a test.")
