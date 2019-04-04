#ifndef GAME_MAINWINDOW_H
#define GAME_MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <vector>
#include "Viewer.hpp"
#include <QKeyEvent>

namespace Ui {
class Game_MainWindow;
}

class Game_MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit Game_MainWindow(QWidget *parent = nullptr);
    ~Game_MainWindow();

protected:
    // Fonction de gestion d'interactions clavier
    void keyPressEvent(QKeyEvent *event);

private:
    Ui::Game_MainWindow *ui;

    void game_begin();

    void createActions();
    void createMenu();

    // Each menu itself
    QMenu* m_menu_app;
    QMenu* m_menu_draw;
    QMenu* m_menu_speed;

    std::vector<QAction*> m_menu_actions;
    std::vector<QAction*> m_menu_actions1;
    Viewer* m_viewer;

private slots:
    void capFarme();
    void app_newgame();
    void updateScore();

    //speed of game
    void speed_slow();
    void speed_medium();
    void speed_fast();

};

#endif // GAME_MAINWINDOW_H
