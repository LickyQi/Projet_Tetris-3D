#include "game_mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    //Initialisation du generateur de nombres aleatoires
    srand(time(nullptr));

    // Creation de l'application QT
    QApplication a(argc, argv);
    Game_MainWindow w;
    w.show();

    // Execution de l'application QT
    return a.exec();
}
