#ifndef GAME_MAINWINDOW_H
#define GAME_MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <vector>
#include "Viewer.hpp"
#include <QKeyEvent>


// @author Qi LI

// Separate UI layout control from other control code
namespace Ui {
class Game_MainWindow;
}

// L'interface principale du jeu
class Game_MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // Constructor
    explicit Game_MainWindow(QWidget *parent = nullptr);
    // Destructor
    ~Game_MainWindow();

protected:
    // Function for keyboard interaction management
    void keyPressEvent(QKeyEvent *event);

private:
    Ui::Game_MainWindow *ui;

    // Begin the game
    void game_begin();

    // Create a new action for quiting and pushes it onto the menu actions vector
    void createActions();
    // Create the menu 'Application' and 'Speed'
    void createMenu();

    // Each menu itself
    QMenu* m_menu_app;
    QMenu* m_menu_draw;
    QMenu* m_menu_speed;

    // Create a menu corresponding action
    std::vector<QAction*> m_menu_actions;
    std::vector<QAction*> m_menu_actions1;
    // Add a viewer to show the game
    Viewer* m_viewer;

private slots:
    // Screen capture, get two punch position information
    void capFrame();
    // Start a new game
    void app_newgame();
    void updateScore();

    //speed of game
    void speed_slow();
    void speed_medium();
    void speed_fast();

};

#endif // GAME_MAINWINDOW_H
