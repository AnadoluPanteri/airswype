/*
 * Copyright (c) 2011 Nokia Corporation.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <datatypes.h>

class Accelerometer;
class AccelerometerFilter;
class QGraphicsView;
class QGraphicsScene;
class QGraphicsObject;
class QTimer;
class QDeclarativeView;
class QDeclarativeEngine;
class QDeclarativeComponent;
class SwypeElement;
class DatabaseManager;
class FormAnalyzer;

#include <QSystemDeviceInfo>

QTM_USE_NAMESPACE

namespace GE {
    class AudioOut;
    class AudioMixer;
    class AudioBuffer;
}

using namespace AS;


/*!
  \class MainWindow
  \brief Main window of the application. Shows QML in different
         QDeclarativeComponents.
*/
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    /*!
      Constructor
    */
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

private slots:
    /*!
      Creates main QML after splash screen
    */
    void initialize();

    /*!
      Finds next shape from database and rotates shape randomly
    */
    void nextShape();

    /*!
      Starts the game mode
    */
    void start(const QVariant &freeDrawMode);

    /*!
      Stops the game
    */
    void stop();

    /*!
      Plays sounds
    */
    void playSound(int index);

    /*!
      Stops playing sounds
    */
    void stopSounds();

    /*!
      Stops sounds if silent profile activated
    */
    void handleProfile(QSystemDeviceInfo::Profile profile);

    /*!
      Removes dublicate points
    */
    void mergeDublicatePoints( Shape &shape );

    /*!
      Analyzes drawed shape and calculates score
    */
    void analyzeShapeData(Shape &shape);

    /*!
      Deletes splash screen
    */
#ifndef Q_WS_HARMATTAN
    void deleteSplash();
#endif

signals:
    void measureStopped();

protected:
    QSystemDeviceInfo* m_SystemDeviceInfo;
    Accelerometer* m_Sensor;
    AccelerometerFilter* m_Filter;

#ifdef Q_WS_HARMATTAN
    QDeclarativeView *m_View;
#else
    QGraphicsView* m_View;
    QGraphicsScene* m_Scene;
    QDeclarativeEngine* m_Engine;

    // QML components
    QDeclarativeComponent* m_SplashComponent;
    QDeclarativeComponent* m_MainComponent;

    // Splash graphics object
    QGraphicsObject* m_SplashQML;
#endif

    // Pointers to QML elements
    SwypeElement* m_targetSwype;
    SwypeElement* m_resultSwype;
    QObject* m_infoView;
    QObject* m_scoreText;

    // Holds the target shape data read from DB
    Shape m_targetShape;

    // Analyzer
    FormAnalyzer *m_FormAnalyzer;

    // Handles I/O to DB
    DatabaseManager* m_DBManager;

    // Game Enabler audio engine classes
    GE::AudioOut* m_AudioOut;
    GE::AudioMixer* m_Mixer;

    // Audio samples
    GE::AudioBuffer* m_SoundMenuMusic;
    GE::AudioBuffer* m_SoundMenuPress;
    GE::AudioBuffer* m_SoundCountDown;

    // Cheer sounds
    QList<GE::AudioBuffer*> m_SoundCheers;
    QList<GE::AudioBuffer*>::Iterator m_SoundCurrentCheerIter;

    // Fail sounds
    QList<GE::AudioBuffer*> m_SoundsFails;
    QList<GE::AudioBuffer*>::Iterator m_SoundCurrentFailIter;

    // Used to tick the audio engine
    QTimer* m_AudioPullTimer;

    // Holds the current game mode
    bool m_FreeDrawMode;

    // For the auto end game logix and score calculation
    bool m_LevelPassed;
    float m_MaxScore;
    int m_MaxScoreShapeCoordinateCount;

    // Attributes
    // How close points in a shape needs to be before they get
    // merged into a single point
    static const float DUPLICATE_EPSILON;
    static const float SCORE_LIMIT;
};

#endif // MAINWINDOW_H
