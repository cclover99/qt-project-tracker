#include "mainwindow.hpp"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget* parent): QMainWindow(parent) {
    setWindowTitle("Project Tracker");
    resize(900, 600);

    auto* centralWidget = new QWidget(this);
    auto* layout = new QVBoxLayout(centralWidget);

    auto* title = new QLabel("Project Tracker");
    auto* button = new QPushButton("Add Project");

    layout->addWidget(title);
    layout->addWidget(button);
    layout->addStretch();

    setCentralWidget(centralWidget);
}