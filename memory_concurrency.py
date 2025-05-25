import threading
import weakref
import time
import gc

class Task:
    def __init__(self, data):
        self.data = data
        print(f"[Created] Task with data: {data}")

    def process(self):
        print(f"[Processing] {self.data}")
        time.sleep(1)
        print(f"[Done] {self.data}")

    def __del__(self):
        print(f"[Deleted] Task with data: {self.data}")

# Shared list to hold weak references to tasks
task_refs = []

# Worker function for threads
def worker(task_ref):
    task = task_ref()
    if task:
        task.process()
    else:
        print("[Skipped] Task was garbage collected")

def main():
    for i in range(5):
        task = Task(f"Item-{i}")
        task_ref = weakref.ref(task)  # Only a weak reference is saved
        task_refs.append(task_ref)

        t = threading.Thread(target=worker, args=(task_ref,))
        t.start()

        # Remove strong reference to allow GC
        del task

    time.sleep(2)  # Let threads finish
    gc.collect()   # Force garbage collection

    print("\n[Status] Remaining tasks:")
    for ref in task_refs:
        if ref() is None:
            print("• Collected")
        else:
            print(f"• Alive: {ref().data}")

if __name__ == "__main__":
    main()
