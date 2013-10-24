AirsWype Example
================

AirSwype is a Qt game example where the user must draw the presented figure in
the air with the phone as precisely as possible.

The application has two modes: a game mode and a free drawing mode. In game 
mode, you try to draw the target shape shown by the red line in the air with 
the phone. In free draw mode, you can practice drawing in the air and study 
how the phone detects the movement. 

The example has been tested to work with Qt 4.7.4. and Qt Mobility 1.2.1 on 
Symbian Anna and MeeGo 1.2 Harmattan.


PREREQUISITES
-------------------------------------------------------------------------------

- Qt basics
- Qt Quick basics


IMPORTANT FILES/CLASSES
-------------------------------------------------------------------------------

Qt C++ files:
- main.cpp: Executes the application
- mainwindow.h: Shows splash screen QML and main page QML
- swypeelement.h: Custom QML element for showing the drawn shape
- databasemanager.h: Reads different shapes from db
- accelerometer.h: Reads accelerometer data
- accelerometerfilter.h: Filters accelerometer data
- formanalyzer.h: Analyses accelerometer data

QML files:
- SplashScreen.qml: Splash screen
- Main.qml: Main QML file
- CountDownView.qml: Shows countdown to start
- TargetView.qml: Shows target of drawing
- ResultView.qml: Shows drawing result and score
- InfoView.qml: Shows application info
- MovingCloud.qml: Clouds
- Sun.qml: Let the sun shine

Important classes: QAccelerometer, QAccelerometerFilter, QSqlDatabase, 
QDeclarativeComponent, QDeclarativeEngine.


REQUIRED CAPABILITIES
-------------------------------------------------------------------------------

None.


KNOWN ISSUES
-------------------------------------------------------------------------------

None.


BUILD & INSTALLATION INSTRUCTIONS
-------------------------------------------------------------------------------

Preparations
~~~~~~~~~~~~

Check that you have the latest Qt/Qt SDK installed in the development 
environment and on the device.


Build & installation instructions using Qt SDK
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

1. Open the Qt PRO file.
   File > Open File or Project, select pro-file.

2. Select target(s), for example 'Qt 4.7.4 for Symbian Anna (Qt SDK)', and
   press the Finish button.

3. Press the Run button to build the project and create the Symbian install 
   package. The application is installed on the device.


COMPATIBILITY
-------------------------------------------------------------------------------

- Qt SDK 1.1
- Qt Mobility 1.2.1
- Qt 4.7.4

Tested on: 
- Nokia C7-00
- Nokia E7-00
- Nokia N8-00
- Nokia X7-00
- Nokia N950

Developed with:
- Qt SDK 1.1


CHANGE HISTORY
-------------------------------------------------------------------------------
1.1.1 Added Gaming profile for Harmattan to support volume keys.
1.1   Added MeeGo 1.2 Harmattan support
1.0   First version
