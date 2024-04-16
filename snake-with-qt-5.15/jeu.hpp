#ifndef JEU_HPP
#define JEU_HPP

#include<list>


typedef enum {VIDE, MUR} Case;
typedef enum {GAUCHE, DROITE, HAUT, BAS} Direction;

class Position
{
  public:
    int x, y;
    Position();
    Position(int,int);
    bool operator==(const Position &) const;
    bool operator!=(const Position &) const;
};

class Jeu
{
  protected:
    Case *terrain;
    Case **terrains;
    int largeur, hauteur; // Nombre de cases en largeur et en hauteur
    std::list<Position> snake;
    Direction dirSnake;
    Position fruite;

    // Variable define number of terrain of the snake game
    int nb_terrains = 5;
    // Array of pointers '*ptTerrain[]' point to an array of 2D 'const char' arrays
    // Using this pointer makes changing between terrain easier
    const char (*ptTerrain[5])[15][21];

  public:
    Jeu();
    Jeu(const Jeu &);
    ~Jeu();

    Jeu &operator=(const Jeu &);

    bool init();
    void evolue();

    // Retourne les dimensions (en nombre de cases)
    int getNbCasesX() const;
    int getNbCasesY() const;

    // Retourne la case à une position donnée
    Case getCase(const Position &) const;

    // Retourne la liste des éléments du serpent en lecture seule
    const std::list<Position> &getSnake() const;

    // Indique si la case à une position donnée existe et est libre
    bool posValide(const Position &) const;

    // Modifie la direction
    void setDirection(Direction);
    const Direction getDirection() const;

    // Add and remove walls
    void ajoutMur();
    void suppressionMur();

    // Function to verify is there a collision:
    void collision();

    //Rewrite function for snake mouvement
    Position seBalader();

    // ** FRUITS **
    //
    const Position &getFruite() const;
    Position genererRandomPosFruite();

    // Index to randomize the fruit pixmap [from 1 to 4]
    int nbRandomFruite = 0;

    // Function to increase the size of snake
    void grandirSnake();

    // ** SCORE **
    int score;

    // ** PLAYING ZONE - TERRAIN **
    // By default, set up a list of 5 different terrains
    void initListTerrain();

    // Using an index to define current terrain used
    int id_terrain = 0;



};

#endif
