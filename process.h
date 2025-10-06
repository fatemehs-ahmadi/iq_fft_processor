#ifndef PROCESS_H
#define PROCESS_H

#include <QString>
#include <QVector>
#include <QPointF>
#include <complex>
#include "ui_mainwindow.h"


class MainWindow;   // Forward declaration to avoid including full MainWindow header

class Process
{
public:
    explicit Process(MainWindow* parent);       //Constructor: parent Pointer to MainWindow, used for UI updates (e.g., progress bar, chart).
    void processFile(const QString &filePath);  //Reads an IQ file, processes its data, and prepares spectrum points.

private:
    MainWindow* m_parent;   //Pointer back to MainWindow for communicating results and status

    /**
     * @brief Compute FFT of complex IQ data.
     * @param iq Input complex vector (time-domain samples).
     * @param fftSize FFT length (number of points).
     * @return Frequency-domain spectrum as a vector of complex values.
     */
    QVector<std::complex<double>> computeFFT(const QVector<std::complex<double>> &iq, int fftSize);
};

#endif // PROCESS_H
