#include "chartmanager.h"
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <algorithm> // for std::minmax_element

/**
 * @brief Constructor for ChartManager.
 *
 * Initializes the chart, axes, and series, and attaches them to the provided
 * QChartView. Sets up titles, hides legend, and enables antialiasing for smoother visuals.
 *
 * @param view The QChartView that will display the chart.
 */
ChartManager::ChartManager(QChartView *view)
    : chart(new QChart),
    series(new QLineSeries),
    axisX(new QValueAxis),
    axisY(new QValueAxis),
    chartView(view)
{
     // Add series (FFT magnitude or PSD data) to chart
    chart->addSeries(series);

    // Attach X and Y axes
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    // Title and labels
    chart->setTitle("Signal Spectrum");
    axisX->setTitleText("Frequency (MHz)");
    axisY->setTitleText("PSD power spectral density(dB)");

    // Hide legend
    chart->legend()->hide();

    // Apply chart to the given view
    chartView->setChart(chart);

    // Enable antialiasing for smooth line rendering
    chartView->setRenderHint(QPainter::Antialiasing);
}

//user input Yaxis range (frequency)
void ChartManager::configureYAxes(double yMin, double yMax) {
    axisY->setRange(yMin, yMax);
}

/**
 * @brief Update chart with new data and auto-scale axes.
 *
 * Replaces current series data with new (x,y) points and automatically adjusts
 * both axes. A small padding is applied to avoid data points touching axis borders.
 *
 * @param data Vector of QPointF containing frequency (X) and power in dB (Y).
 */
void ChartManager::updateChart(const QVector<QPointF> &data) {
    //clears and appends
    series->replace(data);

    if (!data.isEmpty()) {
        // X-axis
        double minX = data.first().x();
        double maxX = data.last().x();

        // paddingX
        double paddingX = (maxX - minX) * 0.01;
        if (paddingX <= 0.0) paddingX = 0.01;
        axisX->setRange(minX - paddingX, maxX + paddingX);

        // Y-axis
        auto [minIt, maxIt] = std::minmax_element(data.begin(), data.end(),
                                                  [](const QPointF &a, const QPointF &b) { return a.y() < b.y(); });

        double minY = minIt->y();
        double maxY = maxIt->y();

        // paddingY
        double paddingY = (maxY - minY) * 0.10;
        if (paddingY <= 0.0) paddingY = 1.0;

        axisY->setRange(minY - paddingY, maxY + paddingY);
    }
}

/**
 * @brief Clear chart data and reset axis ranges.
 *
 * Removes all series points and sets default axis ranges:
 * - X-axis: -1 to 1 (placeholder frequency range)
 * - Y-axis: -100 to 0 (common dB scale for spectrum plots)
 */
void ChartManager::clearChart() {
    series->clear();
    axisX->setRange(-1, 1);
    axisY->setRange(-100, 0);

}
