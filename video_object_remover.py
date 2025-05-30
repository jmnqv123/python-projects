import cv2
import numpy as np
from tkinter import Tk, filedialog, Button, Label
import threading
import os

class VideoObjectRemover:
    def __init__(self, root):
        self.root = root
        self.root.title("Video Object Remover")
        self.label = Label(root, text="Upload a video to begin")
        self.label.pack()

        Button(root, text="Upload Video", command=self.load_video).pack(pady=5)
        Button(root, text="Start Object Removal", command=self.start_removal).pack(pady=5)
        Button(root, text="Exit", command=root.quit).pack(pady=5)

        self.video_path = None
        self.roi = None
        self.valid_roi = False

    def load_video(self):
        self.video_path = filedialog.askopenfilename(
            title="Select Video File",
            filetypes=[("MP4 files", "*.mp4"), ("AVI files", "*.avi"), ("All files", "*.*")]
        )

        if not self.video_path or not os.path.exists(self.video_path):
            self.label.config(text="Video file not selected or doesn't exist.")
            return

        self.label.config(text=f"Loaded: {self.video_path}")
        self.get_roi()

    def get_roi(self):
        cap = cv2.VideoCapture(self.video_path)
        ret, frame = cap.read()
        cap.release()

        if ret:
            self.label.config(text="Select object to remove")
            self.roi = cv2.selectROI("Select Object", frame, False, False)
            cv2.destroyAllWindows()
            x, y, w, h = self.roi
            self.valid_roi = w > 0 and h > 0
            if self.valid_roi:
                self.label.config(text="ROI selected. Ready to remove object.")
            else:
                self.label.config(text="Invalid selection. Try again.")
        else:
            self.label.config(text="Failed to read the video for ROI.")

    def start_removal(self):
        if not self.video_path or not self.valid_roi:
            self.label.config(text="Please upload video and select a valid object first.")
            return

        self.label.config(text="Processing, please wait...")
        threading.Thread(target=self.remove_object_from_video).start()

    def remove_object_from_video(self):
        cap = cv2.VideoCapture(self.video_path)
        fourcc = cv2.VideoWriter_fourcc(*'mp4v')
        fps = cap.get(cv2.CAP_PROP_FPS)
        width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
        height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))

        out_path = filedialog.asksaveasfilename(
            defaultextension=".mp4",
            filetypes=[("MP4 files", "*.mp4")],
            title="Save Processed Video As"
        )

        if not out_path:
            self.label.config(text="Output file not selected.")
            cap.release()
            return

        out = cv2.VideoWriter(out_path, fourcc, fps, (width, height))
        x, y, w, h = self.roi

        frame_count = 0
        while True:
            ret, frame = cap.read()
            if not ret:
                break

            mask = np.zeros(frame.shape[:2], dtype=np.uint8)
            mask[y:y+h, x:x+w] = 255

            inpainted = cv2.inpaint(frame, mask, 3, cv2.INPAINT_TELEA)
            out.write(inpainted)
            frame_count += 1

        cap.release()
        out.release()
        self.label.config(text=f"Done! {frame_count} frames processed. Saved to: {out_path}")
        cv2.destroyAllWindows()

if __name__ == "__main__":
    root = Tk()
    app = VideoObjectRemover(root)
    root.mainloop()

