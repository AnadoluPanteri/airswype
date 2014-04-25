# Copyright (c) 2011-2014 Microsoft Mobile.

QT += core gui declarative sql multimedia

TARGET = airswype
TEMPLATE = app

VERSION = 1.1.1

HEADERS += \
    mainwindow.h \
    datatypes.h \
    accelerometerfilter.h \
    accelerometer.h \
    swypeelement.h \
    databasemanager.h \
    formanalyzer.h \
    gesrc/GEAudioBuffer.h \
    gesrc/GEAudioBufferPlayInstance.h \
    gesrc/GEAudioMixer.h \
    gesrc/GEAudioOut.h \
    gesrc/GEInterfaces.h \
    gesrc/trace.h

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    accelerometerfilter.cpp \
    accelerometer.cpp \
    swypeelement.cpp \
    databasemanager.cpp \
    formanalyzer.cpp \
    gesrc/GEAudioBuffer.cpp \
    gesrc/GEAudioBufferPlayInstance.cpp \
    gesrc/GEAudioMixer.cpp \
    gesrc/GEAudioOut.cpp \
    gesrc/GEInterfaces.cpp

OTHER_FILES += \
    qml/*.qml

RESOURCES += \
    airswype.qrc

symbian {
    message(Symbian build)

    # For debugging the GE audio engine
    #DEFINES += GE_DEBUG=1

    contains(SYMBIAN_VERSION, Symbian3) {
        # Uses volume hack in GE audio engine to set the volume higher.
        DEFINES += QTGAMEENABLER_USE_VOLUME_HACK

        # Define the preprocessor macro to get the version in our app.
        DEFINES += APP_VERSION=\"$$VERSION\"
    }
    else {
        message(Symbian^1)
        DEFINES += Q_OS_SYMBIAN_1
    }

    CONFIG += mobility
    MOBILITY += sensors systeminfo

    RESOURCES += rsc/symbian.qrc

    TARGET = AirSwype
    TARGET.UID3 = 0xE0020100
    ICON = icons/airswype.svg

    # To lock the application to landscape orientation
    LIBS += -lcone -leikcore -lavkon

    # For the very ugly hack to make the master volume control possible
    contains(DEFINES, QTGAMEENABLER_USE_VOLUME_HACK) {
        warning("Using Symbian volume hack!")
        INCLUDEPATH += /epoc32/include/mmf/common
        INCLUDEPATH += /epoc32/include/mmf/server
        LIBS += -lmmfdevsound
    }

    # More memory for the
    TARGET.EPOCHEAPSIZE = 0x100000 0x2000000
    TARGET.EPOCSTACKSIZE = 0x14000

    # Deploy the sounds to application's private folder
    sound.sources = sounds/*.wav
    sound.path = !:/private/E0020100/sounds

    # Deploy the SQLite DB to application's private folder
    db.sources = db/airswype.db.sqlite
    db.path = !:/private/E0020100

    DEPLOYMENT += sound db
}

# For harmattan.
unix:!symbian:!maemo5 {
    message(Harmattan)
    DEFINES += Q_WS_HARMATTAN

    QT -= multimedia

    CONFIG += mobility
    MOBILITY += sensors systeminfo multimedia

    RESOURCES += rsc/harmattan.qrc

    # Define the preprocessor macro to get the version in our app.
    DEFINES += APP_VERSION=\\\"$$VERSION\\\"

    target.path = /opt/usr/bin/airswype

    desktopfile.files = airswype.desktop
    desktopfile.path = /usr/share/applications

    icon.files = icons/airswype.png
    icon.path = /usr/share/icons/hicolor/64x64/apps

    sound.files = sounds/*.wav
    sound.path = /opt/usr/bin/airswype/sounds

    db.files = db/airswype.db.sqlite
    db.path = /opt/usr/bin/airswype

    # Classify the application as a game to support volume keys on Harmattan.
    gameclassify.files += qtc_packaging/debian_harmattan/airswype.conf
    gameclassify.path = /usr/share/policy/etc/syspart.conf.d
    
    INSTALLS += \
        target \
        desktopfile \
        icon \
        sound \
        db \
        gameclassify
}
