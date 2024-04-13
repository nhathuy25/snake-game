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

    // Fruits pixmap: 4 differents fruits
    QPixmap pixmapFruite[4];

    // Score label:
    QLabel *scoreLabel = new QLabel(this);

  public:
    SnakeWindow(QWidget *pParent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags());

  protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);
    void handleTimer();
    void handleButtonAjout();
    void handleButtonSuppr();
    void handleScoreLabel();

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
