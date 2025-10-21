import tkinter as tk
from tkinter import filedialog, ttk
import threading
import librosa
import librosa.display
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import sounddevice as sd
import numpy as np
import time

class AudioAnalyzerApp:
    def __init__(self, root):
        self.root = root
        self.root.title("🎧 Industrial Audio Analyzer")
        self.root.geometry("1000x750")
        self.file_path = None
        self.audio_data = None
        self.sr = None
        self.stream = None
        self.is_playing = False
        self.log = []

        self.setup_ui()

    def setup_ui(self):
        # تغيير الألوان لتكون مريحة وأكثر هدوءًا
        self.root.config(bg="#f7f7f7")  # الخلفية الرئيسية تكون لونه هادئ
        default_font = ("Arial", 12)

        top_frame = tk.Frame(self.root, pady=10, bg="#f7f7f7")
        top_frame.pack()

        tk.Button(top_frame, text="📂 Open Audio File", font=default_font, command=self.open_file, bg="#74b9ff", fg="white", relief="flat", activebackground="#0984e3").pack(side='left', padx=10)

        self.play_button = tk.Button(top_frame, text="▶ Play", font=default_font, command=self.toggle_play, state='disabled', bg="#00b894", fg="white", relief="flat", activebackground="#00a081")
        self.play_button.pack(side='left', padx=5)

        self.stop_button = tk.Button(top_frame, text="⏹ Stop", font=default_font, command=self.stop_audio, state='disabled', bg="#d63031", fg="white", relief="flat", activebackground="#e74c3c")
        self.stop_button.pack(side='left', padx=5)

        self.analyze_button = tk.Button(top_frame, text="🔍 Analyze Full Audio", font=default_font, command=self.analyze_audio, state='disabled', bg="#6c5ce7", fg="white", relief="flat", activebackground="#5e54e3")
        self.analyze_button.pack(side='left', padx=5)

        # إضافة تبويبات خاصة لكل صفحة
        self.tabs = ttk.Notebook(self.root)
        self.tabs.pack(expand=1, fill='both')

        # تبويب Waveform
        self.wave_tab = tk.Frame(self.tabs, bg="#f7f7f7")
        self.tabs.add(self.wave_tab, text="Waveform")
        self.figure_wave = plt.Figure(figsize=(10, 4.5), dpi=100)
        self.ax_wave = self.figure_wave.add_subplot(111)
        self.canvas_wave = FigureCanvasTkAgg(self.figure_wave, self.wave_tab)
        self.canvas_wave.get_tk_widget().pack(fill='both', expand=True)

        # تبويب Spectrogram
        self.spec_tab = tk.Frame(self.tabs, bg="#f7f7f7")
        self.tabs.add(self.spec_tab, text="Spectrogram")
        self.figure_spec = plt.Figure(figsize=(10, 4.5), dpi=100)
        self.ax_spec = self.figure_spec.add_subplot(111)
        self.canvas_spec = FigureCanvasTkAgg(self.figure_spec, self.spec_tab)
        self.canvas_spec.get_tk_widget().pack(fill='both', expand=True)

        # تبويب التحليل
        self.analysis_tab = tk.Frame(self.tabs, bg="#f7f7f7")
        self.tabs.add(self.analysis_tab, text="Analysis")
        self.figure_analysis = plt.Figure(figsize=(10, 4.5), dpi=100)
        self.ax_analysis = self.figure_analysis.add_subplot(111)
        self.canvas_analysis = FigureCanvasTkAgg(self.figure_analysis, self.analysis_tab)
        self.canvas_analysis.get_tk_widget().pack(fill='both', expand=True)

        # تبويب الإشعارات
        self.notif_tab = tk.Frame(self.tabs, bg="#f7f7f7")
        self.tabs.add(self.notif_tab, text="Notifications")
        notif_frame = tk.Frame(self.notif_tab, bg="#f7f7f7")
        notif_frame.pack(fill='both', expand=True)
        self.notif_text = tk.Text(notif_frame, height=15, font=("Courier", 10), bg="#f1f1f1", fg="black", wrap='word')
        self.notif_text.pack(side='left', fill='both', expand=True)
        scrollbar = tk.Scrollbar(notif_frame, command=self.notif_text.yview)
        scrollbar.pack(side='right', fill='y')
        self.notif_text.config(yscrollcommand=scrollbar.set)

        # شريط الحالة
        self.status_var = tk.StringVar()
        self.status_var.set("Status: Ready")
        self.status_bar = tk.Label(self.root, textvariable=self.status_var, bd=1, relief='sunken', anchor='w', font=default_font, bg="#dfe6e9", fg="#2d3436")
        self.status_bar.pack(fill='x')

    def open_file(self):
        path = filedialog.askopenfilename(filetypes=[("Audio Files", "*.wav *.mp3")])
        if path:
            self.file_path = path
            self.status_var.set("Loading audio...")
            self.play_button.config(state='disabled')
            self.analyze_button.config(state='disabled')
            threading.Thread(target=self.load_audio, args=(path,)).start()

    def load_audio(self, path):
        try:
            self.audio_data, self.sr = librosa.load(path, sr=None)
            self.root.after(0, self.plot_waveform)
            self.root.after(0, self.plot_spectrogram)
            self.root.after(0, self.play_button.config, {'state': 'normal'})
            self.root.after(0, self.analyze_button.config, {'state': 'normal'})
            self.root.after(0, self.status_var.set, "Status: File loaded")
        except Exception as e:
            self.root.after(0, self.status_var.set, f"Error: {e}")

    def plot_waveform(self):
        self.ax_wave.clear()
        librosa.display.waveshow(self.audio_data, sr=self.sr, ax=self.ax_wave, color='navy')
        self.ax_wave.set_title("Waveform", fontsize=14)
        self.canvas_wave.draw()

    def plot_spectrogram(self):
        self.ax_spec.clear()
        D = librosa.amplitude_to_db(np.abs(librosa.stft(self.audio_data)), ref=np.max)
        librosa.display.specshow(D, sr=self.sr, x_axis='time', y_axis='log', ax=self.ax_spec, cmap='magma')
        self.ax_spec.set_title("Spectrogram", fontsize=14)
        self.canvas_spec.draw()

    def toggle_play(self):
        if not self.is_playing:
            self.is_playing = True
            self.stop_button.config(state='normal')
            self.play_button.config(state='disabled')
            self.status_var.set("Status: Playing")
            threading.Thread(target=self.play_audio).start()

    def play_audio(self):
        start_time = time.time()

        def callback(outdata, frames, time_info, status):
            if not self.is_playing:
                raise sd.CallbackStop()

            out = self.audio_data[self.frame_pos:self.frame_pos + frames]
            if len(out) < frames:
                out = np.pad(out, (0, frames - len(out)))
                self.is_playing = False
            outdata[:] = out.reshape(-1, 1)
            self.frame_pos += frames

            energy = np.sum(out ** 2)
            try:
                f0 = librosa.yin(out, fmin=50, fmax=2000, sr=self.sr)
                pitch = np.median(f0[np.isfinite(f0)])
            except:
                pitch = 0.0

            if energy > 0.01 and pitch > 0:
                timestamp = time.time() - start_time
                log_msg = f"[{timestamp:.2f}s] Energy: {energy:.4f}, Pitch: {pitch:.2f} Hz"
                self.log.append(log_msg)
                self.root.after(0, self.update_notifications, log_msg)

        self.frame_pos = 0
        with sd.OutputStream(channels=1, callback=callback, samplerate=self.sr):
            while self.is_playing:
                sd.sleep(100)

        self.root.after(0, self.stop_audio)

    def stop_audio(self):
        self.is_playing = False
        self.play_button.config(state='normal')
        self.stop_button.config(state='disabled')
        self.status_var.set("Status: Stopped")

    def update_notifications(self, msg):
        self.notif_text.insert('end', msg + "\n")
        self.notif_text.see('end')

    def analyze_audio(self):
        self.ax_analysis.clear()
        hop_length = 512
        energy = np.array([np.sum(np.square(self.audio_data[i:i + hop_length])) for i in range(0, len(self.audio_data), hop_length)])
        times = librosa.frames_to_time(np.arange(len(energy)), sr=self.sr, hop_length=hop_length)

        try:
            pitches, _ = librosa.piptrack(y=self.audio_data, sr=self.sr, hop_length=hop_length)
            pitch_values = np.max(pitches, axis=0)
        except:
            pitch_values = np.zeros_like(energy)

        # رسم الطاقة و التردد
        self.ax_analysis.plot(times, energy, label="Energy", color='teal')
        self.ax_analysis.plot(times, pitch_values, label="Pitch", color='darkred', alpha=0.7)
        self.ax_analysis.set_title("Sound Changes over Time", fontsize=14)
        self.ax_analysis.set_xlabel("Time (s)")
        self.ax_analysis.set_ylabel("Value")
        self.ax_analysis.legend()
        self.ax_analysis.grid(True)
        self.canvas_analysis.draw()

        for t, e, p in zip(times, energy, pitch_values):
            if e > 0.01 and p > 0:
                log_msg = f"[~{t:.2f}s] Significant change - Energy: {e:.4f}, Pitch: {p:.2f}"
                self.update_notifications(log_msg)

if __name__ == '__main__':
    root = tk.Tk()
    app = AudioAnalyzerApp(root)
    root.mainloop()
