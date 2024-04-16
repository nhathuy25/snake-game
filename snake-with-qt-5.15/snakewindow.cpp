#include <iostream>
#include "snakewindow.hpp"

using namespace std;

SnakeWindow::SnakeWindow(QWidget *pParent, Qt::WindowFlags flags):QFrame(pParent, flags)
{
    // Taille des cases en pixels
    int largeurCase, hauteurCase;

    if (pixmapCorps.load("./data/snake_corps.png")==false)
    {
        cout<<"Impossible d'ouvrir snake_corps.png"<<endl;
        exit(-1);
    }

    if (pixmapTete.load("./data/snake_tete.png")==false)
    {
        cout<<"Impossible d'ouvrir snake_tete.png"<<endl;
        exit(-1);
    }

    if (pixmapMur.load("./data/mur.bmp")==false)
    {
        cout<<"Impossible d'ouvrir mur.bmp"<<endl;
        exit(-1);
    }

    // Adding pixmap for fruits
    if (pixmapFruite[0].load("./data/fruite1.png")==false)
    {
        cout<<"Impossible d'ouvrir fruite1.png"<<endl;
        exit(-1);
    }

    if (pixmapFruite[1].load("./data/fruite2.png")==false)
    {
        cout<<"Impossible d'ouvrir fruite2.png"<<endl;
        exit(-1);
    }

    if (pixmapFruite[2].load("./data/fruite3.png")==false)
    {
        cout<<"Impossible d'ouvrir fruite3.png"<<endl;
        exit(-1);
    }

    if (pixmapFruite[3].load("./data/fruite4.png")==false)
    {
        cout<<"Impossible d'ouvrir fruite4.png"<<endl;
        exit(-1);
    }

    // Initializing the game
    jeu.init();

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &SnakeWindow::handleTimer);
    timer->start(150);

    // Adding button to randomly put a wall
    SnakeButton *btnAjout = new SnakeButton(this);
    btnAjout->setFixedSize(100, 25);
    btnAjout->setText("Ajout mur");
    btnAjout->move(200, 10);

    // Adding button to randomly remove a wall
    SnakeButton *btnSuppr = new SnakeButton(this);
    btnSuppr->setFixedSize(100, 25);
    btnSuppr->setText("Suppr mur");
    btnSuppr->move(325, 10);

    // Connect existing buttons with it's functions
    connect(btnAjout, &QPushButton::clicked, this, &SnakeWindow::handleButtonAjout);
    connect(btnSuppr, &QPushButton::clicked, this, &SnakeWindow::handleButtonSuppr);

    largeurCase = pixmapMur.width();
    hauteurCase = pixmapMur.height();

    // Resize the gaming zone to add buttons on top:
    decalageY = 50;
    resize(jeu.getNbCasesX()*largeurCase, jeu.getNbCasesY()*hauteurCase+decalageY);


    //------ Score label ----------
    scoreLabel->setText("Score: 000");
    scoreLabel->setMargin(12);
    // Setting font's properties for label
    QFont font = scoreLabel->font();
    font.setPointSize(14);
    scoreLabel->setFont(font);
    // Connect the score label with timer to update the score
    connect(timer, &QTimer::timeout, this, &SnakeWindow::handleScoreLabel);

    //------ Changing terrain button ---------
    SnakeButton *btnChangeTerrain = new SnakeButton(this);
    btnChangeTerrain->setFixedSize(100,25);
    btnChangeTerrain->setText("Change terrain");
    btnChangeTerrain->move(450,10);

    // Connect the button to it's active function
    connect(btnChangeTerrain, &QPushButton::clicked, this, &SnakeWindow::handleButtonChangeTerrain);

}

void SnakeWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    Position pos;

    // Taille des cases en pixels
    int largeurCase, hauteurCase;

    largeurCase = pixmapMur.width();
    hauteurCase = pixmapMur.height();

    // Dessine les cases
    for (pos.y=0;pos.y<jeu.getNbCasesY();pos.y++)
        for (pos.x=0;pos.x<jeu.getNbCasesX();pos.x++)
			if (jeu.getCase(pos)==MUR)
                painter.drawPixmap(pos.x*largeurCase, pos.y*hauteurCase+decalageY, pixmapMur);

    // Dessine le serpent
    const list<Position> &snake = jeu.getSnake();
    if (!snake.empty())
    {
        list<Position>::const_iterator itSnake;
        const Position &posTete = snake.front();
        painter.drawPixmap(posTete.x*largeurCase, posTete.y*hauteurCase+decalageY, pixmapTete);

        for (itSnake=++snake.begin(); itSnake!=snake.end(); itSnake++)
            painter.drawPixmap(itSnake->x*largeurCase, itSnake->y*hauteurCase+decalageY, pixmapCorps);
    }

    // Dessin le fruite
    // - randomize the fruit pixmap:
    int nbRandomFruite = jeu.nbRandomFruite;
    Position posFruite = jeu.getFruite();
    painter.drawPixmap(posFruite.x*largeurCase, posFruite.y*largeurCase+decalageY, pixmapFruite[nbRandomFruite]);

}

void SnakeWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key()==Qt::Key_Left && jeu.getDirection()!=DROITE)
        jeu.setDirection(GAUCHE);
    else if (event->key()==Qt::Key_Right && jeu.getDirection()!=GAUCHE)
        jeu.setDirection(DROITE);
    else if (event->key()==Qt::Key_Up && jeu.getDirection()!=BAS)
        jeu.setDirection(HAUT);
    else if (event->key()==Qt::Key_Down && jeu.getDirection()!=HAUT)
        jeu.setDirection(BAS);
    update();
}

void SnakeWindow::handleTimer()
{
    jeu.evolue();
    update();
}

void SnakeWindow::handleButtonAjout()
{
    jeu.ajoutMur();
    update();
}

void SnakeWindow::handleButtonSuppr()
{
    jeu.suppressionMur();
    update();
}

void SnakeWindow::handleScoreLabel()
{
    scoreLabel->setText("Score: " + QString::number(jeu.score));
    update();
}

void SnakeWindow::handleButtonChangeTerrain()
{
    if(jeu.id_terrain<4)
        jeu.id_terrain++;
    else
        jeu.id_terrain=0;
    jeu.init();
    update();
}
