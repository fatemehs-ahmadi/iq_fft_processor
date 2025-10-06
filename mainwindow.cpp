#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "chartmanager.h"
#include "process.h"
#include <QFileDialog>
#include <QProgressBar>
#include <QVector>
#include <QPointF>
#include <complex>
#include <cmath>
#include <vector>

/**
 * @brief MainWindow constructor.
 * Initializes UI components, chart manager, processor, and sets up signal/slot connections.
 *
 * @param parent Optional parent widget.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    chartManager(nullptr)
{
    ui->setupUi(this);                  // Build the UI from the .ui file

    processor = new Process(this);      // Create the processing backend

    // Initialize chart manager with chartView from UI
    chartManager = new ChartManager(ui->chartView);

    // Reset progress bar to 0%
    ui->progressBar->setValue(0);

    // Disable Start button until a file is selected
    ui->startButton->setEnabled(false);
    connect(ui->filePathEdit, &QLineEdit::textChanged, this, [this]() {
        ui->startButton->setEnabled(!ui->filePathEdit->text().isEmpty());
    });

    // Connect UI actions to their slots
    connect(ui->browseButton, &QPushButton::clicked, this, &MainWindow::onBrowseClicked);
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    connect(ui->autoYCheckBox, &QCheckBox::toggled, this, &MainWindow::onAutoYChanged);
}

// Destructor: Cleans up allocated objects.
MainWindow::~MainWindow() {
    delete chartManager;
    delete ui;
    delete processor;
}

//slot - Opens a file dialog for user to choose a file
void MainWindow::onBrowseClicked() {
    QString fileName = QFileDialog::getOpenFileName(this, "Select File");
    if (!fileName.isEmpty()) {
        ui->filePathEdit->setText(fileName);    // Put chosen file path into text box
    }
}

//Slot - enable/disable Y-axis inputs
void MainWindow::onAutoYChanged(bool checked) {
    ui->minYEdit->setEnabled(!checked);
    ui->maxYEdit->setEnabled(!checked);
}

//slot - Simulates some sample data (a sine wave) to draw
void MainWindow::onStartClicked() {
    ui->progressBar->setValue(0);  //Reset progress bar to 0%
    QString filePath = ui->filePathEdit->text();
    if (filePath.isEmpty()) {
        QMessageBox::warning(this, "No File", "Please select a file first.");
        return;
    }
    processor->processFile(filePath);   // Call the processor to read and process the file
}




