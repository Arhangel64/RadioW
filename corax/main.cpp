#include <QtCore/QCoreApplication>
#include <QtCore/QTimer>

#include <utils/signalcatcher.h>

#include "corax.h"

int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);
    W::SignalCatcher sc(&app);
    
    Corax* corax = new Corax(&app);
    
    QTimer::singleShot(0, corax, SLOT(start()));
    QObject::connect(&app, SIGNAL(aboutToQuit()), corax, SLOT(stop()));

    return app.exec();
}