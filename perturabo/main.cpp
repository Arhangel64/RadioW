#include <QtCore/QCoreApplication>
#include <QtCore/QTimer>

#include <utils/signalcatcher.h>

#include "perturabo.h"

int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);
    W::SignalCatcher sc(&app);
    
    Perturabo* perturabo = new Perturabo(&app);
    
    QTimer::singleShot(0, perturabo, SLOT(start()));
    QObject::connect(&app, SIGNAL(aboutToQuit()), perturabo, SLOT(stop()));

    return app.exec();
}
