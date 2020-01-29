#include "game_mainwindow.h"
#include "ui_game_mainwindow.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/objdetect/objdetect.hpp>
#include <QImage>
#include <cstdio>
#include <iostream>
#include <QTimer>
#include <QDebug>
#include <QVBoxLayout>

using namespace cv;
using namespace std;

VideoCapture cap;
QTimer *timer;
QTimer *timer2;
CascadeClassifier fist_cascade;

Game_MainWindow::Game_MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Game_MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("3D Tetris-master");

    if(!fist_cascade.load( "../projet_game_source/fist_v3.xml" ))
    {
        cerr<<"Error loading haarcascade"<<endl;
        exit(0);
    }

    QGLFormat glFormat;
    glFormat.setVersion(3,3);
    glFormat.setProfile(QGLFormat::CoreProfile);
    glFormat.setSampleBuffers(true);

    QVBoxLayout *layout = new QVBoxLayout;
    m_viewer = new Viewer(glFormat, this);
    layout->addWidget(m_viewer);
    ui->gamewidget->setLayout(layout);
    m_viewer->show();

    createActions();
    createMenu();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(capFrame()));

    timer2 = new QTimer(this);
    connect(timer2, SIGNAL(timeout()), this, SLOT(updateScore()));

}


Game_MainWindow::~Game_MainWindow()
{
    delete ui;
}

void Game_MainWindow::updateScore(){
    if (m_viewer->game->score == -1){
        ui->score->setText("You are a loser!");
    }
    else {
        ui->score->setText("Score : "+QString::number(m_viewer->game->score));
    }
}

void Game_MainWindow::game_begin(){

    cap.open(0);// open the default camera
    if(!cap.isOpened())  // check if we succeeded
    {
        cerr<<"Error openning the default camera"<<endl;
        exit(0);
    }
    timer->start();
}

void Game_MainWindow::capFrame(){

    int distance=100;
    Mat frame,frame_gray;
    std::vector<Rect> fists;
    // Get frame
    cap >> frame;
    // Mirror effect
    cv::flip(frame,frame,1);
    // Convert to gray
    cv::cvtColor(frame,frame_gray,COLOR_BGR2GRAY);

    //-- Detect fists
    fist_cascade.detectMultiScale(frame_gray, fists, 1.1, 4, 0|CV_HAAR_SCALE_IMAGE, Size(60, 60) );

    if (fists.size()>0)
    {
        // Draw green rectangle
        for (int i=0;i<(int)fists.size();i++)
            rectangle(frame,fists[i],Scalar(0,255,0),2);
    }

    if (fists.size()==2)
    {
        if (abs(fists[0].x-fists[1].x)< 100) {
            m_viewer->game->rotateCW();
            m_viewer->update();
            cerr<<"Change"<<endl;
        }
        else if (fists[0].x < fists[1].x) {
            if(fists[0].y < (fists[1].y-distance)){
                m_viewer->game->moveLeft();
                cerr<<"right"<<endl;
            }
            else if((fists[0].y-distance) > fists[1].y){
                cerr<<"left"<<endl;
                m_viewer->game->moveRight();
                m_viewer->update();
            }
        }
        else if (fists[0].x > fists[1].x) {
            if(fists[0].y < (fists[1].y-distance)){
                m_viewer->game->moveRight();
                m_viewer->update();
                cerr<<"left"<<endl;
            }
            else if(fists[0].y > (fists[1].y+distance)){
                cerr<<"right"<<endl;
                m_viewer->game->moveLeft();
                m_viewer->update();
            }
        }
        if (fists[0].y> 365 and fists[1].y > 365){
            cerr << "quickly"<<endl;
            m_viewer->game->drop();
            m_viewer->update();
        }
    }
    // Display frame
    cv::cvtColor(frame,frame,CV_BGR2RGB);
    QImage pImageToDisplay = QImage((const unsigned char*)(frame.data),frame.cols, frame.rows,frame.cols*frame.channels(),QImage::Format_RGB888);
    ui->label->setPixmap(QPixmap::fromImage(pImageToDisplay));
}

void Game_MainWindow::keyPressEvent(QKeyEvent * event){
    m_viewer->mTimer3->stop();
    switch(event->key())
    {

    // Sortie de l'application
    case Qt::Key_Escape:
    {
        QCoreApplication::instance()->quit();
        break;

    }
    case Qt::Key_B:
    {
        game_begin();
        break;
    }
        // Activation/Arret de l'animation
    case Qt::Key_Left:{
        m_viewer->game->moveRight();
        m_viewer->update();
        break;
    }
    case Qt::Key_Right:{
        m_viewer->game->moveLeft();
        m_viewer->update();
        break;
    }
    case Qt::Key_Up: {
        m_viewer->game->rotateCW();
        m_viewer->update();
        break;
    }
    case Qt::Key_Space: {
        m_viewer->game->drop();
        m_viewer->update();
        break;
    }
        // Cas par defaut
    default:
    {
        // Ignorer l'evenement
        event->ignore();
        return;
    }
    }
    // Acceptation de l'evenement et mise a jour de la scene
    event->accept();
}

void Game_MainWindow::createActions() {
    // Creates a new action for quiting and pushes it onto the menu actions vector
    QAction* quitAct = new QAction(tr("&Quit"), this);
    m_menu_actions.push_back(quitAct);

    // We set the accelerator keys
    quitAct->setShortcut(Qt::Key_Q);

    // Set the tip
    quitAct->setStatusTip(tr("Exits the file"));

    // Connect the action with the signal and slot designated
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

    QAction* newgameAct = new QAction(tr("&New game"), this);
    newgameAct->setShortcut(Qt::Key_N);
    newgameAct->setStatusTip(tr("Start a new game"));
    connect(newgameAct, SIGNAL(triggered()), this, SLOT(app_newgame()));
    m_menu_actions.push_back(newgameAct);

    QAction* slowAct = new QAction(tr("&Slow"), this);
    slowAct->setShortcut(Qt::Key_1);
    slowAct->setStatusTip(tr("game speed: slow"));
    connect(slowAct, SIGNAL(triggered()), this, SLOT(speed_slow()));

    QAction* mediumAct = new QAction(tr("&Medium"), this);
    mediumAct->setShortcut(Qt::Key_2);
    mediumAct->setStatusTip(tr("game speed: medium"));
    connect(mediumAct, SIGNAL(triggered()), this, SLOT(speed_medium()));

    QAction* fastAct = new QAction(tr("&Fast"), this);
    fastAct->setShortcut(Qt::Key_3);
    fastAct->setStatusTip(tr("game speed: fast"));
    connect(fastAct, SIGNAL(triggered()), this, SLOT(speed_fast()));

    m_menu_actions1.push_back(slowAct);
    m_menu_actions1.push_back(mediumAct);
    m_menu_actions1.push_back(fastAct);

}

void Game_MainWindow::createMenu() {
    m_menu_app = menuBar()->addMenu(tr("&Application"));
    for (auto& action : m_menu_actions) {
        m_menu_app->addAction(action);
    }

    m_menu_speed = menuBar()->addMenu(tr("&Speed"));
    for (auto& action : m_menu_actions1) {
        m_menu_speed->addAction(action);
    }
}

void Game_MainWindow::app_newgame() {
    timer2->start(500);
    m_viewer->newgame();
}

void Game_MainWindow::speed_slow(){
    m_viewer->mTimer2->start(1000);
}
void Game_MainWindow::speed_medium(){
    m_viewer->mTimer2->start(300);
}
void Game_MainWindow::speed_fast(){
    m_viewer->mTimer2->start(100);
}

