import React, { useState, useEffect } from 'react';
import { io } from 'socket.io-client';

const socket = io("http://localhost:5000");

function App() {
  const [room, setRoom] = useState("general");
  const [username, setUsername] = useState("Guest");
  const [msg, setMsg] = useState("");
  const [messages, setMessages] = useState([]);

  useEffect(() => {
    socket.emit("join", { username, room });
    socket.on("message", (data) => {
      setMessages((prev) => [...prev, `${data.user}: ${data.msg}`]);
    });

    return () => {
      socket.emit("leave", { username, room });
      socket.off();
    };
  }, [room, username]);

  const sendMessage = () => {
    socket.emit("send_message", { room, username, msg });
    setMsg("");
  };

  return (
    <div style={{ padding: 20 }}>
      <h2>Chat Room: {room}</h2>
      <input
        value={username}
        onChange={(e) => setUsername(e.target.value)}
        placeholder="Username"
      /><br/>
      <div style={{ height: 300, overflowY: 'scroll', border: '1px solid gray', margin: '10px 0' }}>
        {messages.map((m, i) => (
          <div key={i}>{m}</div>
        ))}
      </div>
      <input
        value={msg}
        onChange={(e) => setMsg(e.target.value)}
        placeholder="Type a message"
        onKeyDown={(e) => e.key === 'Enter' && sendMessage()}
      />
      <button onClick={sendMessage}>Send</button>
    </div>
  );
}

export default App;

