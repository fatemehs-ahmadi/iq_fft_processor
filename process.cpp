// ============================================================================
// Process::processFile
// Read interleaved int16 IQ from file in blocks, convert to complex IQ pairs,
// apply DC removal and Hann window, compute FFT (via computeFFT), normalize,
// convert to dB, and push the resulting (frequency, dB) points to the chart.
// ============================================================================

#include "process.h"
#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "kiss_fft.h"
#include <QFile>
#include <QDataStream>
#include <QCoreApplication>
#include <QtCharts/QLineSeries>
#include <cmath>

/**
 * @brief Constructor for the Process class
 * @param parent Pointer to the MainWindow to allow chart updates and UI interaction
 */
Process::Process(MainWindow* parent)
    : m_parent(parent)
{
}

/**
 * @brief Computes FFT using KissFFT library
 * @param iq Vector of complex IQ samples
 * @param fftSize Size of the FFT (must be power of 2)
 * @return Vector of complex FFT results
 */
QVector<std::complex<double>> Process::computeFFT(const QVector<std::complex<double>> &iq, int fftSize)
{
    QVector<kiss_fft_cpx> in(fftSize), out(fftSize);

    // Copy IQ samples to KissFFT input (zero-padding if needed)
    for (int i = 0; i < fftSize; ++i) {
        if (i < iq.size()) {
            in[i].r = static_cast<float>(iq[i].real());
            in[i].i = static_cast<float>(iq[i].imag());
        } else {
            in[i].r = 0.0f;
            in[i].i = 0.0f;
        }
    }

    // Allocate FFT configuration
    kiss_fft_cfg cfg = kiss_fft_alloc(fftSize, 0, nullptr, nullptr);
    if (!cfg) return {};

    // Perform FFT
    kiss_fft(cfg, in.data(), out.data());
    free(cfg);

    // Convert output to std::complex
    QVector<std::complex<double>> spectrum(fftSize);
        for (int i = 0; i < fftSize; ++i)
        spectrum[i] = { out[i].r, out[i].i };

    return spectrum;
}

/**
 * @brief Main processing function
 * Reads IQ data from file, slices blocks for FFT, applies DC removal and window,
 * computes FFT, normalizes magnitude, converts to dB, and updates the chart.
 *
 * @param filePath Path to the IQ file
 */
void Process::processFile(const QString &filePath)
{
    if (!m_parent) return;    // Safety check

    // Open IQ file for reading
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) return;

    QDataStream in(&file);
    in.setByteOrder(QDataStream::LittleEndian);     // Ensure correct byte order

    // Processing Configuration
    const int blockSize = 30000;             // Number of int16 values to read per block
    const int fftSize   = 8192;              // FFT size (must be power of 2)

    // Frequency axis (Hz) computed from UI parameters
    double f0 = m_parent->getCenterFreq();   // Center frequency [Hz]
    double bw = m_parent->getBandwidth();    // Bandwidth [Hz]
    double fStart = f0 - bw / 2.0;           // Start frequency [Hz]
    double fStep  = bw / fftSize;            // Frequency bin step

    QVector<QPointF> spectrumPoints;         // Stores FFT points(frequency, magnitude in dB) points for the chart

    // Precompute Hann window to reduce spectral leakage
    QVector<double> window(fftSize);
    for (int n = 0; n < fftSize; ++n) {
        window[n] = 0.5 * (1.0 - std::cos(2.0 * M_PI * n / (fftSize - 1)));
    }

    // Main processing loop
    while (!in.atEnd()) {

        // Read a block of raw IQ data (interleaved int16: I,Q,I,Q,...)
        QVector<qint16> raw(blockSize);
        int bytesRead = in.readRawData(reinterpret_cast<char*>(raw.data()), blockSize * sizeof(qint16));

        // Stop if we couldn't read a full block
        if (bytesRead < blockSize * sizeof(qint16)) break;

        // Convert to complex<double> IQ samples
        QVector<std::complex<double>> iq;
        iq.reserve(blockSize / 2);
        for (int i = 0; i < blockSize / 2; ++i) {
            iq.append(std::complex<double>(
                static_cast<double>(raw[2*i]),
                static_cast<double>(raw[2*i+1])
                ));
        }

        // Ensure enough samples for FFT
        if (iq.size() < fftSize) break;
        QVector<std::complex<double>> slice = iq.mid(0, fftSize);

        // DC removal (subtract mean of the slice)
        std::complex<double> mean(0.0, 0.0);
        for (auto &s : slice) mean += s;
        mean /= static_cast<double>(slice.size());
        for (auto &s : slice) s -= mean;

        // Apply Hann window
        for (int i = 0; i < fftSize; ++i) slice[i] *= window[i];

        // Compute FFT
        QVector<std::complex<double>> fftResult = computeFFT(slice, fftSize);

        // Perform FFT shift (move zero frequency to center)
        QVector<std::complex<double>> fftShifted(fftSize);
        int half = fftSize / 2;
        fftShifted = fftResult;
        std::rotate(fftShifted.begin(), fftShifted.begin() + half, fftShifted.end());


        // Compute magnitude spectrum in dB and map to frequency axis
        spectrumPoints.clear();
        spectrumPoints.reserve(fftSize);
        for (int i = 0; i < fftSize; ++i) {
            double mag = std::abs(fftShifted[i]) /  static_cast<double>(fftSize);    //normalize
            double magdB = 20.0 * log10(mag + 1e-12);           // dB scale
            double freq  = fStart + i * fStep;                  // Frequency axis
            spectrumPoints.append(QPointF(freq, magdB));  // Convert to MHz
        }

        // Update chart with current FFT
        m_parent->getChartManager()->updateChart(spectrumPoints);

        // Apply fixed Y-axis limits if Auto-Y is disabled
        if (!m_parent->isAutoY()) {
            double minY = m_parent->getMinY();
            double maxY = m_parent->getMaxY();
            m_parent->getChartManager()->configureYAxes(minY, maxY);
        }

        // Update progress bar
        double progress = static_cast<double>(file.pos()) / file.size();
        if (file.pos() % (10 * blockSize * sizeof(qint16)) == 0)
            m_parent->setProgress(static_cast<int>(progress * 100));

        // Allow Qt UI to remain responsive during long processing
        QCoreApplication::processEvents();
    }

    file.close();   // Close file when finished
}
