/**
 * Copyright (c) 2011 Nokia Corporation.
 */

#include <QtGui>
#include "mainwindow.h"

// Lock orientation in Symbian
#ifdef Q_OS_SYMBIAN
    #include <eikenv.h>
    #include <eikappui.h>
    #include <aknenv.h>
    #include <aknappui.h>
#endif


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

#ifndef Q_OS_SYMBIAN_1
    app.setApplicationVersion(APP_VERSION);
#endif

    // Lock orientation in Symbian
#ifdef Q_OS_SYMBIAN
        CAknAppUi* appUi =
                dynamic_cast<CAknAppUi*> (CEikonEnv::Static()->AppUi());
        TRAP_IGNORE(
            if (appUi) {
                appUi->SetOrientationL(CAknAppUi::EAppUiOrientationPortrait);
            }
        );
#endif

    MainWindow mainWindow;

    mainWindow.setGeometry(QApplication::desktop()->screenGeometry());
    mainWindow.showFullScreen();

    return app.exec();
}
