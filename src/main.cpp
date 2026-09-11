#include <QApplication>
#include <QFontDatabase>

#include "mainwindow.hpp"
#include "database.hpp"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QFont appFont = QApplication::font();
    appFont.setFamilies({"Segoe UI Variable", "Segoe UI"});

    QApplication::setFont(appFont);

    if (!Database::initialize())
        return 1;

    MainWindow window;
    window.show();

    return app.exec();
}