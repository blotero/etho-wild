#include <QApplication>
#include <QFont>
#include "MainWindow.hpp"
#include "ThemeManager.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // Set application metadata for QSettings
    app.setOrganizationName("EthoWild");
    app.setApplicationName("EthoWild");
    
    // Apply saved theme before showing any windows
    ThemeManager::instance().applyTheme();
    
    MainWindow window;
    window.show();
    
    return app.exec();
}

