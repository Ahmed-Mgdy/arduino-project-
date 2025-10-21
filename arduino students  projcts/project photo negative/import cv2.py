import tkinter as tk
from tkinter import filedialog, messagebox, ttk
from PIL import Image, ImageTk
import cv2
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import os

class ImageApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Image Negative DSP Project")
        self.root.geometry("1100x650")
        self.root.configure(bg="#6A5ACD")

        self.image = None
        self.negative = None
        self.filename = ""

        button_frame = tk.Frame(root, bg="#6A5ACD")
        button_frame.pack(pady=10)

        style = {'font': ('Arial', 10, 'bold'), 'bg': '#483D8B', 'fg': 'white'}

        tk.Button(button_frame, text="Choose Image", command=self.load_image, **style).grid(row=0, column=0, padx=5)
        tk.Button(button_frame, text="Cancel", command=self.cancel_image, **style).grid(row=0, column=1, padx=5)
        tk.Button(button_frame, text="Show Statistics", command=self.show_statistics, **style).grid(row=0, column=2, padx=5)
        tk.Button(button_frame, text="Show Plots", command=self.show_all_plots, **style).grid(row=0, column=3, padx=5)
        tk.Button(button_frame, text="Save Negative Image", command=self.save_negative, **style).grid(row=0, column=4, padx=5)

        self.original_panel = tk.Label(root, text="Original Image", bg="#6A5ACD", fg="white", font=("Arial", 12))
        self.original_panel.pack(side="left", padx=20)

        self.negative_panel = tk.Label(root, text="Negative Image", bg="#6A5ACD", fg="white", font=("Arial", 12))
        self.negative_panel.pack(side="right", padx=20)

    def load_image(self):
        path = filedialog.askopenfilename(filetypes=[("Image Files", "*.png *.jpg *.jpeg *.bmp *.tiff"), ("All Files", "*.*")])
        if not path:
            return

        self.filename = os.path.basename(path)
        self.image = cv2.imread(path)
        self.negative = 255 - self.image

        self.show_image(self.image, self.original_panel)
        self.show_image(self.negative, self.negative_panel)

    def cancel_image(self):
        self.image = None
        self.negative = None
        self.filename = ""
        self.original_panel.config(image='', text="Original Image")
        self.negative_panel.config(image='', text="Negative Image")

    def show_image(self, img, panel):
        img_rgb = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
        img_pil = Image.fromarray(img_rgb).resize((400, 300))
        img_tk = ImageTk.PhotoImage(img_pil)
        panel.configure(image=img_tk, text='')
        panel.image = img_tk

    def show_statistics(self):
        if self.image is None:
            messagebox.showwarning("Warning", "Please choose an image first.")
            return

        min_vals = self.image.min(axis=(0, 1))
        max_vals = self.image.max(axis=(0, 1))
        mean_vals = self.image.mean(axis=(0, 1))

        stats = f"""
Blue:   Min = {min_vals[0]:.0f}, Max = {max_vals[0]:.0f}, Mean = {mean_vals[0]:.2f}
Green:  Min = {min_vals[1]:.0f}, Max = {max_vals[1]:.0f}, Mean = {mean_vals[1]:.2f}
Red:    Min = {min_vals[2]:.0f}, Max = {max_vals[2]:.0f}, Mean = {mean_vals[2]:.2f}
"""
        messagebox.showinfo("Image Statistics", stats)

    def show_all_plots(self):
        if self.image is None:
            messagebox.showwarning("Warning", "Please choose an image first.")
            return

        plot_window = tk.Toplevel(self.root)
        plot_window.title("Image Plots")
        plot_window.geometry("900x700")
        plot_window.configure(bg="#6A5ACD")

        notebook = ttk.Notebook(plot_window)
        notebook.pack(fill='both', expand=True)

        # Histogram Tab
        fig1 = plt.Figure(figsize=(7, 5))
        ax1 = fig1.add_subplot(111)
        for i, color in enumerate(['b', 'g', 'r']):
            hist = cv2.calcHist([self.image], [i], None, [256], [0, 256])
            ax1.plot(hist, color=color)
        ax1.set_title("Histogram")
        ax1.set_xlabel("Pixel Value")
        ax1.set_ylabel("Frequency")
        ax1.grid(True)
        tab1 = tk.Frame(notebook)
        notebook.add(tab1, text="Histogram")
        canvas1 = FigureCanvasTkAgg(fig1, master=tab1)
        canvas1.draw()
        canvas1.get_tk_widget().pack()

        def open_hist_window():
            fig, ax = plt.subplots()
            for i, color in enumerate(['b', 'g', 'r']):
                hist = cv2.calcHist([self.image], [i], None, [256], [0, 256])
                ax.plot(hist, color=color)
            ax.set_title("Histogram")
            ax.set_xlabel("Pixel Value")
            ax.set_ylabel("Frequency")
            ax.grid(True)
            plt.show()

        tk.Button(tab1, text="Pop out", command=open_hist_window, bg="#483D8B", fg="white").pack(pady=5)

        # Bar Chart Tab
        fig2 = plt.Figure(figsize=(7, 5))
        ax2 = fig2.add_subplot(111)
        min_vals = self.image.min(axis=(0, 1))
        max_vals = self.image.max(axis=(0, 1))
        mean_vals = self.image.mean(axis=(0, 1))
        x = np.arange(3)
        width = 0.25
        ax2.bar(x - width, min_vals, width, label="Min", color=['blue', 'green', 'red'])
        ax2.bar(x, mean_vals, width, label="Mean", color=['blue', 'green', 'red'])
        ax2.bar(x + width, max_vals, width, label="Max", color=['blue', 'green', 'red'])
        ax2.set_xticks(x)
        ax2.set_xticklabels(['Blue', 'Green', 'Red'])
        ax2.set_title("Channel Statistics")
        ax2.legend()
        ax2.grid(True)
        tab2 = tk.Frame(notebook)
        notebook.add(tab2, text="Bar Chart")
        canvas2 = FigureCanvasTkAgg(fig2, master=tab2)
        canvas2.draw()
        canvas2.get_tk_widget().pack()

        def open_bar_chart_window():
            fig, ax = plt.subplots()
            ax.bar(x - width, min_vals, width, label="Min", color=['blue', 'green', 'red'])
            ax.bar(x, mean_vals, width, label="Mean", color=['blue', 'green', 'red'])
            ax.bar(x + width, max_vals, width, label="Max", color=['blue', 'green', 'red'])
            ax.set_xticks(x)
            ax.set_xticklabels(['Blue', 'Green', 'Red'])
            ax.set_title("Channel Statistics")
            ax.legend()
            ax.grid(True)
            plt.show()

        tk.Button(tab2, text="Pop out", command=open_bar_chart_window, bg="#483D8B", fg="white").pack(pady=5)

        # Boxplot Tab
        fig3 = plt.Figure(figsize=(7, 5))
        ax3 = fig3.add_subplot(111)
        ax3.boxplot([
            self.image[:, :, 0].ravel(),
            self.image[:, :, 1].ravel(),
            self.image[:, :, 2].ravel()
        ], labels=['Blue', 'Green', 'Red'])
        ax3.set_title("Boxplot")
        ax3.set_ylabel("Pixel Intensity")
        ax3.grid(True)
        tab3 = tk.Frame(notebook)
        notebook.add(tab3, text="Boxplot")
        canvas3 = FigureCanvasTkAgg(fig3, master=tab3)
        canvas3.draw()
        canvas3.get_tk_widget().pack()

        def open_boxplot_window():
            fig, ax = plt.subplots()
            ax.boxplot([
                self.image[:, :, 0].ravel(),
                self.image[:, :, 1].ravel(),
                self.image[:, :, 2].ravel()
            ], labels=['Blue', 'Green', 'Red'])
            ax.set_title("Boxplot")
            ax.set_ylabel("Pixel Intensity")
            ax.grid(True)
            plt.show()

        tk.Button(tab3, text="Pop out", command=open_boxplot_window, bg="#483D8B", fg="white").pack(pady=5)

    def save_negative(self):
        if self.negative is None:
            messagebox.showwarning("Warning", "No image to save.")
            return

        save_path = f"negative_{self.filename}"
        cv2.imwrite(save_path, self.negative)
        messagebox.showinfo("Saved", f"Negative image saved as: {save_path}")


if __name__ == "__main__":
    root = tk.Tk()
    app = ImageApp(root)
    root.mainloop()
