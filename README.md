# iq_fft_processor

A Qt C++ application that reads binary IQ signal files (`.iq`), processes them in 30,000-sample blocks, computes 8192-point FFT spectra, and visualizes the results in real-time charts. Supports auto or manual Y-axis scaling for signal analysis.

---

## Features

- Reads raw interleaved Int16 IQ samples from binary files.
- Processes data in configurable chunks.
- Applies DC removal and Hann window to reduce spectral leakage.
- Computes FFT (using KissFFT) and normalizes magnitude.
- Displays frequency vs. power (dB) in interactive charts.
- Supports auto-scaling or fixed Y-axis range.
- Progress bar shows real-time processing progress.

---

## Installation

1. **Clone the repository**
```bash
git clone https://github.com/YOUR_USERNAME/iq_fft_processor.git
cd iq_fft_processor
```
2. **Open in Qt Creator**

-Open the .pro project file.

-Configure kit for your compiler (MSVC, MinGW, etc.).

3. **Build and Run**

-Click Run in Qt Creator.

-Select an IQ file to process using the Browse button.

-Press Start to compute and display the spectrum

## Usage

1. Place your IQ file in a convenient location.

2. Launch the application.

3. Click Browse to select your IQ file.

4. Press Start to process data.

5. Adjust the Y-axis manually or use auto-scaling for display.

## Project Structure

- main.cpp – Launches Qt application and main window.

- mainwindow.* – UI logic and interaction.

- chartmanager.* – Handles chart creation and updating.

- process.* – Reads IQ data, performs FFT, and updates chart.

- kiss_fft.h/.c – FFT library used for spectral analysis.

- .pro – QMake project file.

## License

This project is licensed under the MIT License.

## Notes

- FFT size and block size are configurable in process.cpp.

- Designed for educational and research purposes in signal processing.

- Real-time plotting allows monitoring of spectrum as the file is processed.
