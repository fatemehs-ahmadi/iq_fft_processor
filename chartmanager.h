#ifndef CHARTMANAGER_H
#define CHARTMANAGER_H
#include <QMainWindow>
#include <QVector>
#include <QPointF>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

class ChartManager {
public:
    ChartManager(QChartView *view);                 // Construct a ChartManager instance.
    void updateChart(const QVector<QPointF> &data); // Update the chart with new data points.
    void configureYAxes(double yMin, double yMax);  //User configure the Y-axis range.
    void clearChart();                              //Resets the X and Y axis range and clears all data points from the series.

private:
    QChart *chart;          // The chart object containing series and axes
    QChartView *chartView;  // The widget used to render the chart
    QLineSeries *series;    // Data series (line) displayed on the chart
    QValueAxis *axisX;      // X-axis of the chart
    QValueAxis *axisY;      // Y-axis of the chart
};

#endif // CHARTMANAGER_H
