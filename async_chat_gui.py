import asyncio
import sys
import tkinter as tk
from datetime import datetime
import threading
import queue

# ------------------ Server ------------------

async def handle_client(reader, writer):
    addr = writer.get_extra_info('peername')
    print(f"[+] Client connected: {addr}")

    try:
        while True:
            data = await reader.read(100)
            if not data:
                print(f"[-] Client disconnected: {addr}")
                break

            message = data.decode().strip()
            timestamp = datetime.now().strftime("%H:%M:%S")
            print(f"[{timestamp}] [{addr}] {message}")

            response = f"[{timestamp}] Echo: {message}"
            writer.write(response.encode())
            await writer.drain()
    except Exception as e:
        print(f"[!] Error with {addr}: {e}")
    finally:
        writer.close()
        await writer.wait_closed()

async def start_server():
    server = await asyncio.start_server(handle_client, '127.0.0.1', 8888)
    addr = server.sockets[0].getsockname()
    print(f"[*] Server running on {addr}")

    async with server:
        await server.serve_forever()

# ------------------ GUI Client ------------------

class AsyncChatClient(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Async Chat Client")
        self.geometry("500x400")

        self.msg_frame = tk.Frame(self)
        self.msg_frame.pack(pady=10)

        self.text_area = tk.Text(self.msg_frame, height=15, width=58, state=tk.DISABLED)
        self.text_area.pack(side=tk.LEFT, fill=tk.BOTH)

        scrollbar = tk.Scrollbar(self.msg_frame)
        scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
        self.text_area.config(yscrollcommand=scrollbar.set)
        scrollbar.config(command=self.text_area.yview)

        self.entry = tk.Entry(self, width=48)
        self.entry.pack(pady=10, side=tk.LEFT, padx=(10,0))
        self.entry.bind("<Return>", lambda event: self.send_message())

        self.send_button = tk.Button(self, text="Send", command=self.send_message)
        self.send_button.pack(pady=10, side=tk.LEFT, padx=(5,10))

        self.queue = queue.Queue()
        self.protocol("WM_DELETE_WINDOW", self.on_close)

        self.loop = asyncio.new_event_loop()
        self.reader = None
        self.writer = None
        self.running = True

        threading.Thread(target=self.start_async_loop, daemon=True).start()
        self.after(100, self.process_incoming)

    def start_async_loop(self):
        asyncio.set_event_loop(self.loop)
        self.loop.run_until_complete(self.tcp_client())

    async def tcp_client(self):
        try:
            self.reader, self.writer = await asyncio.open_connection('127.0.0.1', 8888)
            self.queue.put("[*] Connected to server.\n")

            while self.running:
                data = await self.reader.read(100)
                if not data:
                    self.queue.put("[*] Server closed the connection.\n")
                    break
                msg = data.decode()
                self.queue.put(msg + "\n")
        except Exception as e:
            self.queue.put(f"[!] Connection error: {e}\n")

    def send_message(self):
        message = self.entry.get().strip()
        if message and self.writer:
            timestamp = datetime.now().strftime("%H:%M:%S")
            display_msg = f"[{timestamp}] You: {message}\n"
            self.text_area.config(state=tk.NORMAL)
            self.text_area.insert(tk.END, display_msg)
            self.text_area.config(state=tk.DISABLED)
            self.text_area.see(tk.END)

            # Send async without blocking Tkinter
            asyncio.run_coroutine_threadsafe(self.async_send(message), self.loop)
            self.entry.delete(0, tk.END)

    async def async_send(self, message):
        if self.writer:
            self.writer.write(message.encode())
            await self.writer.drain()

    def process_incoming(self):
        while not self.queue.empty():
            msg = self.queue.get_nowait()
            self.text_area.config(state=tk.NORMAL)
            self.text_area.insert(tk.END, msg)
            self.text_area.config(state=tk.DISABLED)
            self.text_area.see(tk.END)
        if self.running:
            self.after(100, self.process_incoming)

    def on_close(self):
        self.running = False
        if self.writer:
            self.writer.close()
            self.loop.call_soon_threadsafe(self.loop.stop)
        self.destroy()

# ------------------ Main ------------------

def print_usage():
    print("Usage: python async_chat_gui.py [server|client]")
    print("Run as server or client.")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print_usage()
        sys.exit(1)

    mode = sys.argv[1].lower()

    if mode == 'server':
        asyncio.run(start_server())
    elif mode == 'client':
        app = AsyncChatClient()
        app.mainloop()
    else:
        print_usage()
        sys.exit(1)
