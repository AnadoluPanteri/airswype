/*
 * Copyright (c) 2011 Nokia Corporation.
 */

#include <QtGui>
#include <QtDeclarative>
#include "mainwindow.h"
#include "databasemanager.h"
#include "swypeelement.h"
#include "formanalyzer.h"
#include "gesrc/GEAudioOut.h"
#include "gesrc/GEAudioMixer.h"
#include "gesrc/GEAudioBuffer.h"
#include "gesrc/GEAudioBufferPlayInstance.h"

#include "accelerometer.h"
#include "accelerometerfilter.h"

using namespace GE;

const float MainWindow::DUPLICATE_EPSILON = 0.025f;
const float MainWindow::SCORE_LIMIT = 90.0f;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
#ifdef Q_WS_HARMATTAN
    m_View = new QDeclarativeView(this);
#else
    m_Scene = new QGraphicsScene(this);
    m_View = new QGraphicsView(m_Scene, this);

    m_View->setAttribute(Qt::WA_NoSystemBackground);

    // Optimization flags
    m_View->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    m_View->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    m_View->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_View->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_Scene->setItemIndexMethod(QGraphicsScene::NoIndex);

    // QML key handling options, same that QDeclarativeView uses
    m_View->viewport()->setFocusPolicy(Qt::NoFocus);
    m_View->setFocusPolicy(Qt::StrongFocus);
    m_Scene->setStickyFocus(true);

    m_Engine = new QDeclarativeEngine(m_View);
#endif

    setCentralWidget(m_View);

#ifdef Q_WS_HARMATTAN
    initialize();
#else
    // Shows splash screen
    m_SplashComponent =
            new QDeclarativeComponent(m_Engine,
                                      QUrl("qrc:/qml/SplashScreen.qml"));

    m_SplashQML =
            qobject_cast<QGraphicsObject*>(m_SplashComponent->create());

    m_Scene->addItem(m_SplashQML);

    // Initialize application main view after showing splash screen
    QTimer::singleShot(0, this, SLOT(initialize()));
#endif
}


MainWindow::~MainWindow()
{
}


void MainWindow::initialize()
{
    // Register the custom QML element
    qmlRegisterType<SwypeElement>("SwypeElements", 1, 0, "Swype");

#ifdef Q_WS_HARMATTAN
    m_View->setSource(QUrl("qrc:/qml/Main.qml"));
    QDeclarativeItem *mainQML = qobject_cast<QDeclarativeItem*>(m_View->rootObject());
#else
     // Create the main QML Declarative component, this loads fast
    m_MainComponent = new QDeclarativeComponent(m_Engine,
                                                QUrl("qrc:/qml/Main.qml"));

    // This is the slow operation, could this be done in worker thread?
    QDeclarativeItem *mainQML =
            qobject_cast<QDeclarativeItem*>(m_MainComponent->create());
    m_Scene->addItem(mainQML);
#endif

    // Find the interesting QML elements from QML side, we will attach
    // our engine to those elements.
    m_resultSwype = mainQML->findChild<SwypeElement*>("resultSwypeView");
    m_targetSwype = mainQML->findChild<SwypeElement*>("targetSwypeView");
    m_infoView = mainQML->findChild<QObject*>("infoView");
    m_scoreText = mainQML->findChild<QObject*>("scoreText");

    if (!m_resultSwype || !m_targetSwype || !m_scoreText || !m_infoView) {
        QMessageBox::warning(this, tr("Error"),
                             tr("Not all QML elements was found"));
        qApp->quit();
    }

    // Set the version number to infoview
    m_infoView->setProperty("version", qApp->applicationVersion());

    // Set the score limit to QML side
    mainQML->setProperty("scoreLimit", SCORE_LIMIT);

    m_SystemDeviceInfo = new QSystemDeviceInfo(this);
    m_Sensor = new Accelerometer(this);
    m_Filter = new AccelerometerFilter(m_Sensor);
    m_Sensor->addFilter(m_Filter);
    m_Sensor->setDataRate(40);

    // Create form analyzer
    m_FormAnalyzer = new FormAnalyzer(this);

    QString dir = QDir::toNativeSeparators(qApp->applicationDirPath()) + QDir::separator();

    // Create Database manager
    m_DBManager = new DatabaseManager(this);
    if(!m_DBManager->openDB(dir + "airswype.db.sqlite")) {
        qDebug() << m_DBManager->lastError();
    }

    // Create audio classes
    m_Mixer = new AudioMixer(this);
    m_AudioOut = new AudioOut(m_Mixer, this);

    dir = dir + "sounds" + QDir::separator();

    // Load wavs
    m_SoundMenuMusic =
        AudioBuffer::loadWav(dir + "35722__offthesky__wind_chimes.wav");
    m_SoundMenuPress =
        AudioBuffer::loadWav(dir + "60000__qubodup__swosh_13.wav");
    m_SoundCountDown =
        AudioBuffer::loadWav(dir + "92909__Robinhood76__01526_swoosh_2.wav");

    m_SoundCheers.append(
        AudioBuffer::loadWav(dir + "46448__PhreaKsAccount__cheer1.wav"));
    m_SoundCheers.append(
        AudioBuffer::loadWav(dir + "46450__PhreaKsAccount__cheer2.wav"));
    m_SoundCheers.append(
        AudioBuffer::loadWav(dir + "46451__PhreaKsAccount__cheer3.wav"));
    m_SoundCheers.append(
        AudioBuffer::loadWav(dir + "46452__PhreaKsAccount__cheer4.wav"));
    m_SoundCheers.append(
        AudioBuffer::loadWav(dir + "46455__PhreaKsAccount__cheer7.wav"));
    m_SoundCheers.append(
        AudioBuffer::loadWav(dir + "46456__PhreaKsAccount__cheer8.wav"));
    m_SoundCheers.append(
        AudioBuffer::loadWav(dir + "46457__PhreaKsAccount__cheer9.wav"));
    m_SoundCurrentCheerIter = m_SoundCheers.begin();

    m_SoundsFails.append(
        AudioBuffer::loadWav(dir + "46463__PhreaKsAccount__jibberish1.wav"));
    m_SoundsFails.append(
        AudioBuffer::loadWav(dir + "46465__PhreaKsAccount__jibberish2.wav"));
    m_SoundsFails.append(
        AudioBuffer::loadWav(dir + "46466__PhreaKsAccount__jibberish3.wav"));
    m_SoundsFails.append(
        AudioBuffer::loadWav(dir + "46467__PhreaKsAccount__jibberish4.wav"));
    m_SoundsFails.append(
        AudioBuffer::loadWav(dir + "46468__PhreaKsAccount__jibberish5.wav"));
    m_SoundsFails.append(
        AudioBuffer::loadWav(dir + "46469__PhreaKsAccount__jibberish6.wav"));
    m_SoundsFails.append(
        AudioBuffer::loadWav(dir + "46470__PhreaKsAccount__jibberish7.wav"));
    m_SoundsFails.append(
        AudioBuffer::loadWav(dir + "46471__PhreaKsAccount__jibberish8.wav"));
    m_SoundsFails.append(
        AudioBuffer::loadWav(dir + "46472__PhreaKsAccount__jibberish9.wav"));
    m_SoundCurrentFailIter = m_SoundsFails.begin();

    m_AudioPullTimer = new QTimer(this);
    m_AudioPullTimer->setInterval(5);

    connect(mainQML, SIGNAL(nextShape()), this, SLOT(nextShape()));
    connect(this, SIGNAL(measureStopped()), mainQML, SLOT(measureStopped()));
    connect(mainQML, SIGNAL(start(const QVariant&)),
            this, SLOT(start(const QVariant&)));
    connect(m_Sensor, SIGNAL(calibrationDone()),
            mainQML, SLOT(measureStarted()));
    connect(mainQML, SIGNAL(stopMeasure()), this, SLOT(stop()));
    connect(m_Sensor, SIGNAL(shapeData(Shape&)),
            this, SLOT(analyzeShapeData(Shape&)));

    // Silent profile handling
    connect(m_SystemDeviceInfo,
            SIGNAL(currentProfileChanged(QSystemDeviceInfo::Profile)),
            this, SLOT(handleProfile(QSystemDeviceInfo::Profile)));

    // Audio signals and slots
    connect(m_AudioPullTimer, SIGNAL(timeout()), m_AudioOut, SLOT(tick()));
    connect(mainQML, SIGNAL(playSound(int)), this, SLOT(playSound(int)));
    connect(mainQML, SIGNAL(stopAllSounds()), this, SLOT(stopSounds()));

#ifdef Q_WS_HARMATTAN
    connect(m_View->engine(), SIGNAL(quit()), qApp, SLOT(quit()));
#else
    // Framework signals and slots
    connect(m_SplashQML, SIGNAL(hideFinished()), this, SLOT(deleteSplash()));
    connect(m_Engine, SIGNAL(quit()), qApp, SLOT(quit()));
#endif

    // Start the GE audio engine
    m_AudioPullTimer->start();

    // Initialize random generator
    QTime midnight(0, 0, 0);
    qsrand(midnight.secsTo(QTime::currentTime()));

    // Set the first shape in DB as target Swype
    nextShape();

    // Play menu music
    playSound(0);

#ifndef Q_WS_HARMATTAN
    // Start the hiding animation of the splash screen
    QMetaObject::invokeMethod(m_SplashQML, "hide");
#endif
}


void MainWindow::nextShape()
{
    m_targetShape = m_DBManager->nextShape();

    if((qrand() & 255) < 128) {
        // 50% chance to flip shape horizontally
        m_targetShape.flipHorizontally();
    }

    if((qrand() & 255) < 128) {
        // 50% chance to flip shape vertically
        m_targetShape.flipVertically();
    }

    if((qrand() & 255) < 128) {
        // 50% chance to rotate shape 90 degrees
        m_targetShape.rotate90Degrees();
    }

    // Sets timeToShow to the QML element
    m_targetSwype->setProperty("timeToShow", m_targetShape.timeToShow * 1000);
    m_targetSwype->setShapeFromDb(m_targetShape, m_targetShape.timeToShow * 1000);
}


void MainWindow::start(const QVariant &freeDrawMode)
{
    // We get the game mode here, in free draw mode
    // don't compare shapes
    m_FreeDrawMode = freeDrawMode.toBool();

    m_LevelPassed = false;
    m_MaxScore = 0;

    qDebug() << "FreeDrawMode: " << m_FreeDrawMode;

    // Clear the result view with empty data
    Shape shape;
    m_resultSwype->setShape(shape);

    m_Sensor->start();
}


void MainWindow::stop()
{
    m_Sensor->stop();
}


void MainWindow::mergeDublicatePoints( Shape &shape )
{
    Shape::Coordinates newCoordinates;

    qreal mergedPos[3];
    int mergedContributorCount;
    qreal d[3];
    qreal distance;

    Shape::Coordinates::Iterator i=shape.coordinates.begin();

    do
    {
        mergedPos[0] = (*i).pos[0];
        mergedPos[1] = (*i).pos[1];
        mergedPos[2] = (*i).pos[2];
        mergedContributorCount = 1;


        Shape::Coordinates::Iterator iscan=i;

        while (iscan!=shape.coordinates.end())
        {
            iscan++;
            if (iscan!=shape.coordinates.end()) {
                d[0] = (*i).pos[0] - (*iscan).pos[0];
                d[1] = (*i).pos[1] - (*iscan).pos[1];
                d[2] = (*i).pos[2] - (*iscan).pos[2];
                distance = d[0]*d[0] + d[1]*d[1] + d[2]*d[2];
                if (distance < DUPLICATE_EPSILON * DUPLICATE_EPSILON ) {
                    mergedPos[0] += (*iscan).pos[0];
                    mergedPos[1] += (*iscan).pos[1];
                    mergedPos[2] += (*iscan).pos[2];
                    mergedContributorCount++;
                } else break;
            } else break;
        }



        // finished scanning this point
        mergedPos[0] /= (qreal)mergedContributorCount;
        mergedPos[1] /= (qreal)mergedContributorCount;
        mergedPos[2] /= (qreal)mergedContributorCount;

        // Push merged coordinate into the new coordinate-list
        newCoordinates.push_back(Coordinate(mergedPos[0], mergedPos[1], mergedPos[2]));

        i = iscan;      // continue from the next point.
    }
    while (i!=shape.coordinates.end());

    qDebug() << "mergeDuplicatePoints(shape): From "
             << shape.coordinates.count()
             << " to:"
             << newCoordinates.count();
    shape.coordinates = newCoordinates;
}


void MainWindow::analyzeShapeData(Shape &shape)
{
    // Remove duplicate points
    //mergeDublicatePoints( shape );

    // Calculates length of the shape and bounding box
    shape.shapePreAnalyzed = false;
    m_FormAnalyzer->preAnalyzeShape( m_targetShape );
    m_FormAnalyzer->preAnalyzeShape( shape );

    if(shape.coordinates.count() > 500) {
        m_Sensor->stop();
        emit measureStopped();
    }
    else if(shape.totalDistanceTravelled < 0.10) {
        // Compare shapes after the shape is at least 10 cm long
        return;
    }
    else if(m_FreeDrawMode == false)  {
        float score =
                m_FormAnalyzer->compareShapes(m_targetShape, shape) * 100;

        m_scoreText->setProperty("score", score);

        // Automatically end the game if level is analyzed as passed
        if(score > SCORE_LIMIT) {
            // Level is now determined as passed, but the player
            // might still improve the score
            m_LevelPassed = true;
            m_MaxScoreShapeCoordinateCount = shape.coordinates.count();
        }

        if (m_LevelPassed && score < m_MaxScore) {
            // Now the player could not improve the score anymore,
            // end the game
            score = m_MaxScore;
            m_scoreText->setProperty("score", score);

            // Cut the points away from shape's coordinates
            // that lower the result.
            while(shape.coordinates.count() > m_MaxScoreShapeCoordinateCount) {
                shape.coordinates.pop_back();
            }

            m_Sensor->stop();

            emit measureStopped();
        }
        else {
            if (score > m_MaxScore) {
                // The player improves the score,
                // remember the position (iterator)
                m_MaxScore = score;
                m_MaxScoreShapeCoordinateCount = shape.coordinates.count();
            }
        }
    }

    m_resultSwype->setShape(shape);
}


void MainWindow::playSound(int index)
{
    if(m_SystemDeviceInfo->currentProfile() ==
            QSystemDeviceInfo::SilentProfile) {
        return;
    }

    switch(index) {
    case 0:
        m_SoundMenuMusic->playWithMixer(*m_Mixer);
        break;
    case 1:
        m_SoundMenuPress->playWithMixer(*m_Mixer);
        break;
    case 2:
        m_SoundCountDown->playWithMixer(*m_Mixer);
        break;
    case 3:
        (*m_SoundCurrentCheerIter)->playWithMixer(*m_Mixer);
        m_SoundCurrentCheerIter++;

        if(m_SoundCurrentCheerIter == m_SoundCheers.end()) {
            m_SoundCurrentCheerIter = m_SoundCheers.begin();
        }
        break;
    case 4:
        (*m_SoundCurrentFailIter)->playWithMixer(*m_Mixer);
        m_SoundCurrentFailIter++;

        if(m_SoundCurrentFailIter == m_SoundsFails.end()) {
            m_SoundCurrentFailIter = m_SoundsFails.begin();
        }
        break;
    default:
        return;
    };
}


void MainWindow::stopSounds()
{
    m_Mixer->destroyList();
}


#ifndef Q_WS_HARMATTAN
void MainWindow::deleteSplash()
{
    if(m_SplashComponent) {
        delete m_SplashComponent;
        m_SplashComponent = NULL;
        m_SplashQML = NULL;
    }
}
#endif


void MainWindow::handleProfile(QSystemDeviceInfo::Profile profile)
{
    if(profile == QSystemDeviceInfo::SilentProfile) {
        stopSounds();
    }
}
