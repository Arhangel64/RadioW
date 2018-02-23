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
    MainWindow* wnd = new MainWindow();;
    
    QObject::connect(roboute, SIGNAL(debugMessage(const QString&)), wnd, SLOT(robouteMessage(const QString&)));
    QObject::connect(roboute, SIGNAL(newService(uint64_t, const QString&)), wnd, SLOT(newService(uint64_t, const QString&)));
    QObject::connect(roboute, SIGNAL(serviceConnecting(uint64_t)), wnd, SLOT(serviceConnecting(uint64_t)));
    QObject::connect(roboute, SIGNAL(serviceConnected(uint64_t)), wnd, SLOT(serviceConnected(uint64_t)));
    QObject::connect(roboute, SIGNAL(serviceDisconnecting(uint64_t)), wnd, SLOT(serviceDisconnecting(uint64_t)));
    QObject::connect(roboute, SIGNAL(serviceDisconnected(uint64_t)), wnd, SLOT(serviceDisconnected(uint64_t)));
    QObject::connect(roboute, SIGNAL(serviceConnectionFailed(uint64_t)), wnd, SLOT(serviceConnectionFailed(uint64_t)));
    QObject::connect(roboute, SIGNAL(serviceLaunched(uint64_t)), wnd, SLOT(serviceLaunched(uint64_t)));
    QObject::connect(roboute, SIGNAL(serviceStopped(uint64_t)), wnd, SLOT(serviceStopped(uint64_t)));
    QObject::connect(roboute, SIGNAL(serviceLaunching(uint64_t)), wnd, SLOT(serviceLaunching(uint64_t)));
    QObject::connect(roboute, SIGNAL(serviceStopping(uint64_t)), wnd, SLOT(serviceStopping(uint64_t)));
    QObject::connect(roboute, SIGNAL(serviceRemoved(uint64_t)), wnd, SLOT(serviceRemoved(uint64_t)));
    QObject::connect(roboute, SIGNAL(serviceAttrChange(uint64_t, const QString&, const QString&)), 
                     wnd, SLOT(serviceAttrChange(uint64_t, const QString&, const QString&)));
    QObject::connect(roboute, SIGNAL(log(uint64_t, const QString&)), wnd, SLOT(serviceLog(uint64_t, const QString&)));
    QObject::connect(roboute, SIGNAL(serviceAddCommand(uint64_t, const QString&, const QMap<QString, uint64_t>)),
                     wnd, SLOT(serviceAddCommand(uint64_t, const QString&, const QMap<QString, uint64_t>)));
    QObject::connect(roboute, SIGNAL(serviceRemoveCommand(uint64_t, const QString&)),
                     wnd, SLOT(serviceRemoveCommand(uint64_t, const QString&)));
    QObject::connect(roboute, SIGNAL(serviceClearCommands(uint64_t)), wnd, SLOT(serviceClearCommands(uint64_t)));
    QObject::connect(roboute, SIGNAL(serviceChangeName(uint64_t, const QString&)), wnd, SLOT(serviceNameChange(uint64_t, const QString&)));
    QObject::connect(roboute, SIGNAL(serviceEdit(uint64_t, const QMap<QString, QString>&)), wnd, SLOT(serviceEdit(uint64_t, const QMap<QString, QString>&)));
    
    QObject::connect(wnd, SIGNAL(addService(const QMap<QString, QString>&)), roboute, SLOT(addService(const QMap<QString, QString>&)));
    QObject::connect(wnd, SIGNAL(connectService(uint64_t)), roboute, SLOT(connectService(uint64_t)));
    QObject::connect(wnd, SIGNAL(disconnectService(uint64_t)), roboute, SLOT(disconnectService(uint64_t)));
    QObject::connect(wnd, SIGNAL(launchService(uint64_t)), roboute, SLOT(launchService(uint64_t)));
    QObject::connect(wnd, SIGNAL(stopService(uint64_t)), roboute, SLOT(stopService(uint64_t)));
    QObject::connect(wnd, SIGNAL(removeService(uint64_t)), roboute, SLOT(removeService(uint64_t)));
    QObject::connect(wnd, SIGNAL(launchCommand(uint64_t, const QString&, const QMap<QString, QVariant>&)), 
                     roboute, SLOT(launchCommand(uint64_t, const QString&, const QMap<QString, QVariant>&)));
    QObject::connect(wnd, SIGNAL(editService(uint64_t)), roboute, SLOT(editService(uint64_t)));
    QObject::connect(wnd, SIGNAL(changeService(uint64_t, const QMap<QString, QString>&)), roboute, SLOT(changeService(uint64_t, const QMap<QString, QString>&)));
    
    
    QTimer::singleShot(0, roboute, SLOT(start()));
    QObject::connect(&app, SIGNAL(aboutToQuit()), roboute, SLOT(stop()));
    QObject::connect(&app, SIGNAL(aboutToQuit()), wnd, SLOT(saveSettings()));

    wnd->show();
    int result = app.exec();
    delete wnd;
    
    return result;
}
