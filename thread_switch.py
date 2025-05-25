import threading
import time

def worker(name):
    for i in range(5):
        print(f"[{name}] Working on step {i}")
        time.sleep(0.5)  # Forces the thread to yield and switch

def main():
    t1 = threading.Thread(target=worker, args=("Thread-1",))
    t2 = threading.Thread(target=worker, args=("Thread-2",))

    t1.start()
    t2.start()

    t1.join()
    t2.join()

    print("All threads finished.")

if __name__ == "__main__":
    main()
