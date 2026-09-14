#pragma once

#include <QMainWindow>

#include "database.hpp"

class QVBoxLayout;
class QStackedWidget;
class QPushButton;

class MainWindow : public QMainWindow {
public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    QWidget* addProject(const Project& project);

    QVBoxLayout* sidebarLayout;
    QStackedWidget* projectViews;
    QPushButton* addProjectButton;
};