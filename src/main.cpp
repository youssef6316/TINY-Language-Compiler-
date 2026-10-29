#include <QApplication>
#include "views/MainWindow.h"
#include "utils/Theme.h"

int main(int argc, char *argv[]) {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    // Enable High DPI scaling for Qt 5
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif

    QApplication app(argc, argv);
    app.setApplicationName("TINY Lexical Analyzer Studio");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("Compiler Engineering");

    // Apply custom IDE dark theme stylesheet
    app.setStyleSheet(Theme::getApplicationStyleSheet());

    MainWindow window;
    window.show();

    return app.exec();
}
