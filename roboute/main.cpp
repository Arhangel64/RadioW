#include <QtWidgets/QApplication>
#include <QtCore/QTimer>

#include <utils/signalcatcher.h>

#include "roboute.h"
#include "mainwindow.h"

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("RadioW");
    QCoreApplication::setApplicationName("Roboute");
    QCoreApplication::setApplicationVersion("0.0.1");
    
    W::SignalCatcher sc(&app);
    
    Roboute* roboute = new Roboute(&app);
    MainWindow wnd;
    
    QObject::connect(roboute, SIGNAL(debugMessage(const QString&)), &wnd, SLOT(robouteMessage(const QString&)));
    QObject::connect(roboute, SIGNAL(newService(const Service&)), &wnd, SLOT(newService(const Service&)));
    QObject::connect(&wnd, SIGNAL(addService(const QMap<QString, QString>&)), roboute, SLOT(addService(const QMap<QString, QString>&)));
    
    QTimer::singleShot(0, roboute, SLOT(start()));
    QObject::connect(&app, SIGNAL(aboutToQuit()), roboute, SLOT(stop()));

    wnd.show();
    return app.exec();
}
