#include <iostream>
#include <QtCore/QCoreApplication>

#include "corax.h"

int main(int argc, char **argv) {
	QCoreApplication app(argc, argv);
    
	Corax corax(&app);
	  
	return app.exec();
}