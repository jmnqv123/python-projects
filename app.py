from flask import Flask, render_template
from flask_socketio import SocketIO, send, emit

app = Flask(__name__)
socketio = SocketIO(app)

# Home route to render the chat app page
@app.route('/')
def index():
    return render_template('index.html')

# SocketIO event for sending a message
@socketio.on('message')
def handle_message(msg):
    print('Message: ' + msg)
    send(msg, broadcast=True)  # Broadcast to all clients

if __name__ == '__main__':
    socketio.run(app, debug=True)
