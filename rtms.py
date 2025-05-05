from flask import Flask, render_template
from flask_socketio import SocketIO, send

# Initialize Flask app and SocketIO
app = Flask(__name__)
socketio = SocketIO(app)

@app.route('/')
def index():
    return render_template('index.html')  # Serve the front-end

@socketio.on('message')
def handle_message(msg):
    print(f"Received message: {msg}")  # Print message in server terminal
    send(msg, broadcast=True)  # Broadcast message to all connected clients

if __name__ == '__main__':
    socketio.run(app, debug=True)

