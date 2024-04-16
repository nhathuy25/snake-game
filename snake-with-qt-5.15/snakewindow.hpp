#ifndef SNAKEWINDOW_HPP
#define SNAKEWINDOW_HPP

#include <QtCore/QtCore>
#include <QtWidgets/QtWidgets>
#include "jeu.hpp"

class SnakeWindow : public QFrame
{
  protected:
    Jeu jeu;
    QPixmap pixmapCorps, pixmapTete, pixmapMur;
    int decalageY;

    // Fruits pixmap: 4 different fruits
    QPixmap pixmapFruite[4];

    // Score label:
    QLabel *scoreLabel = new QLabel(this);

    // Timer
    QTimer *timer = new QTimer(this);

  public:
    SnakeWindow(QWidget *pParent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags());


  protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);
    void handleTimer();
    void handleButtonAjout();
    void handleButtonSuppr();
    // Function to display score label by time
    void handleScoreLabel();

    // Function to change between terrain and re-initialize the game afterward
    void handleButtonChangeTerrain();

    // Function to change the speed of the snake.
    void handleButtonChangeSpeed();


    // Set up label to display game over message
    QLabel *gameoverLabel = new QLabel(this);

    // Function 'gestionFinNiveau' assign with timer to detect whether the game is over
    void gestionFinNiveau();
    // Function to display gameover message
    void displayGameoverMessage();
    // Function to restart the game
    void restartGame();
};

class SnakeButton : public QPushButton
{
  public:
    SnakeButton(QWidget *pParent=0):QPushButton(pParent) {}

  protected:
    void keyPressEvent(QKeyEvent *e)
    {
        if (parent()!=NULL)
            QCoreApplication::sendEvent(parent(), e);
    }
};


#endif
