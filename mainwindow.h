#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QtCharts/QtCharts>
#include "chartmanager.h"
#include "process.h"

///< Forward declaration of the auto-generated UI class (from mainwindow.ui).
namespace Ui
{ class MainWindow;
}


/**
 * @class MainWindow
 * @brief The main application window.
 *
 * This class provides the GUI framework for the spectrum analyzer application.
 * It manages the user interface, delegates plotting tasks to ChartManager, and
 * coordinates data processing via the Process class.
 */
class MainWindow : public QMainWindow {
    ///< Qt macro that enables signals, slots, and other meta-object features.
    Q_OBJECT

public:

    //Construct a MainWindow instance (Optional parent widget).
    explicit MainWindow(QWidget *parent = nullptr);

    //Destructor: cleans up resources.
    ~MainWindow();

    //return Center frequency entered by the user.
    double getCenterFreq() const { return ui->centerFreqEdit->text().toDouble(); }

    //return Bandwidth entered by the user.
    double getBandwidth()  const { return ui->bandwidthEdit->text().toDouble(); }

    //return True if "Auto Y-axis" checkbox is checked.
    bool   isAutoY()       const { return ui->autoYCheckBox->isChecked(); }

    //return Minimum Y-axis value entered by the user.
    double getMinY()       const { return ui->minYEdit->text().toDouble(); }

    //return Maximum Y-axis value entered by the user.
    double getMaxY()       const { return ui->maxYEdit->text().toDouble(); }

    //Update the progress bar in the UI.(percentage (0–100))
    void setProgress(int value) { ui->progressBar->setValue(value); }

    // return Pointer to the ChartManager instance.
    ChartManager* getChartManager() const { return chartManager; }

private slots:
    //event handlers connected to UI actions
    void onBrowseClicked();             //file browser
    void onStartClicked();              //start process and draw chart
    void onAutoYChanged(bool checked);  //enable/disable Y-axis inputs

private:
    // UI
    Ui::MainWindow *ui;             // Pointer to the auto-generated UI object

    // Managers
    ChartManager *chartManager;     // Handles plotting of spectrum data

    Process* processor;             // Handles data loading and FFT processing
};

#endif // MAINWINDOW_H
