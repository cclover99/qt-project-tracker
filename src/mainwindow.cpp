#include "mainwindow.hpp"

#include "widgets/projectTitle.hpp"
#include "widgets/projectDescription.hpp"

#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPalette>
#include <QPushButton>
#include <QSplitter>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <QMenu>
#include <QMessageBox>

#include <QApplication>
#include <QStyle>
#include <QIcon>

MainWindow::MainWindow(QWidget* parent): QMainWindow(parent) {
    setWindowTitle("Project Tracker");
    resize(1000, 700);

    auto* splitter = new QSplitter(Qt::Horizontal, this);

    auto* sidebar = new QWidget;
    sidebar->setMinimumWidth(160);

    auto sidebarPalette = sidebar->palette();
    sidebarPalette.setColor(
        QPalette::Window,
        palette().color(QPalette::Window).darker(115)
    );

    sidebar->setPalette(sidebarPalette);
    sidebar->setAutoFillBackground(true);

    sidebarLayout = new QVBoxLayout(sidebar);

    auto* projectsTitle = new QLabel("Projects\n");
    projectsTitle->setStyleSheet(R"(
        QLabel {
            margin-top: 4px;
        }
    )");
    
    QFont titleFont = projectsTitle->font();
    titleFont.setPointSize(12);
    titleFont.setWeight(QFont::DemiBold);
    projectsTitle->setFont(titleFont);
    
    sidebarLayout->addWidget(projectsTitle);

    projectViews = new QStackedWidget;

    auto* emptyView = new QLabel("Select a project");
    emptyView->setAlignment(Qt::AlignCenter);

    projectViews->addWidget(emptyView);

    addProjectButton = new QPushButton("+");

    QFont addProjectFont = addProjectButton->font();
    addProjectFont.setWeight(QFont::DemiBold);

    addProjectButton->setFont(addProjectFont);

    sidebarLayout->addWidget(addProjectButton);

    sidebarLayout->addStretch();

    for (const Project& project : Database::getProjects()) {
        addProject(project);
    }

    connect(addProjectButton, &QPushButton::clicked, this, [this]() {
        bool ok;

        QString name = QInputDialog::getText(
            this,
            "New Project",
            "Project name:",
            QLineEdit::Normal,
            "",
            &ok
        );

        name = name.trimmed();

        if (!ok || name.isEmpty())
            return;

        Project project = Database::createProject(name);

        if (project.id != 0)
            addProject(project);
    });

    splitter->addWidget(sidebar);
    splitter->addWidget(projectViews);

    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);

    splitter->setSizes({220, 780});
    splitter->setChildrenCollapsible(false);

    setCentralWidget(splitter);
}

void MainWindow::addProject(const Project& project) {
    auto* button = new QPushButton(project.name);

    button->setStyleSheet(R"(
        QPushButton {
            border: none;
            background: transparent;
            text-align: left;
            padding: 5px 8px;
        }

        QPushButton:hover {
            background-color: rgba(255, 255, 255, 20);
            border-radius: 5px;
        }
    )");

    // View
    auto* view = new QWidget;
    view->setFocusPolicy(Qt::ClickFocus);
    auto* viewLayout = new QVBoxLayout(view);

    // Set title and name changing behavior
    auto originalName = std::make_shared<QString>(project.name);

    auto* title = new ProjectTitle(project.name);

    connect(
        title,
        &ProjectTitle::renameRequested,
        this,
        [title, button, projectId = project.id] (const QString& oldName, const QString& newName) {
            if (Database::renameProject(projectId, newName)) {
                button->setText(newName);
            } else {
                title->setTitle(oldName);
            }
        }
    );

    viewLayout->addWidget(title, 0, Qt::AlignLeft);

    auto* description = new ProjectDescription(project.description);

    connect(
        description,
        &ProjectDescription::descriptionChanged,
        this,
        [description, projectId = project.id] (const QString& oldDescription, const QString& newDescription) {
            if (!Database::updateDescription(projectId, newDescription)) {
                description->setDescription(oldDescription);
            }
        }
    );

    viewLayout->addWidget(description);

    // Add the view widget
    viewLayout->addStretch();
    projectViews->addWidget(view);

    int addButtonIndex = sidebarLayout->indexOf(addProjectButton);
    sidebarLayout->insertWidget(addButtonIndex, button);

    connect(button, &QPushButton::clicked, this, [this, view]() {
        projectViews->setCurrentWidget(view);
    });

    // Right click context menu
    button->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(
        button,
        &QPushButton::customContextMenuRequested,
        this,
        [this, button, view, project](const QPoint& pos) {
            QMenu menu(button);

            

            QMenu* folderMenu = menu.addMenu(
                QIcon::fromTheme("folder"),
                "Move to folder"
            );
            folderMenu->menuAction()->setEnabled(false);

            QAction* placeholderFolder = folderMenu->addAction("No folders available");
            placeholderFolder->setEnabled(false);

            QAction* deleteAction = menu.addAction(
                QIcon::fromTheme("edit-delete"),
                "Delete project"
            );

            QAction* selectedAction = menu.exec(
                button->mapToGlobal(pos)
            );

            if (selectedAction != deleteAction)
                return;

            QMessageBox::StandardButton result = QMessageBox::question(
                this,
                "Delete Project",
                QString("Are you sure you want to delete \"%1\"?")
                    .arg(project.name),
                QMessageBox::Yes | QMessageBox::No,
                QMessageBox::No
            );

            if (result != QMessageBox::Yes)
                return;

            if (!Database::deleteProject(project.id))
                return;

            if (projectViews->currentWidget() == view)
                projectViews->setCurrentIndex(0);

            projectViews->removeWidget(view);

            view->deleteLater();
            button->deleteLater();
        }
    );
}