#include <iostream>
#include <cassert>
#include "jeu.hpp"

using namespace std;

Position::Position()
{
}

Position::Position(int a, int b)
{
    x=a;
    y=b;
}

bool Position::operator==(const Position &pos) const
{
    return (x==pos.x && y==pos.y);
}

bool Position::operator!=(const Position &pos) const
{
    return (x!=pos.x || y!=pos.y);
}

Jeu::Jeu()
{
    terrain = nullptr;
    largeur = 0; hauteur = 0;
    dirSnake = DROITE;
}

Jeu::Jeu(const Jeu &jeu):snake(jeu.snake)
{
    largeur = jeu.largeur;
    hauteur = jeu.hauteur;
    dirSnake = jeu.dirSnake;

    if (jeu.terrain!=nullptr)
    {
        terrain = new Case[largeur*hauteur];
        for (int c=0; c<largeur*hauteur; c++)
            terrain[c] = jeu.terrain[c];
    }
    else
        terrain = nullptr;
}

Jeu::~Jeu()
{
    if (terrain!=nullptr)
        delete[] terrain;
}

Jeu &Jeu::operator=(const Jeu &jeu)
{
    if (terrain!=nullptr)
        delete[] terrain;

    largeur = jeu.largeur;
    hauteur = jeu.hauteur;
    dirSnake = jeu.dirSnake;
    snake = jeu.snake;

    if (jeu.terrain!=nullptr)
    {
        terrain = new Case[largeur*hauteur];
        for (int c=0; c<largeur*hauteur; c++)
            terrain[c] = jeu.terrain[c];
    }
    else
        terrain = nullptr;
    return *this;
}

bool Jeu::init()
{
	int x, y;
	// list<Position>::iterator itSnake;

	//Initialize the playing zone
	const char terrain_defaut[15][21] = {
		"####..##############",
		"#........##........#",
		"#.#####..##...####.#",
		"#........##........#",
		"#..................#",
		"#..................#",
		"....................",
		"....................",
		"....................",
		"....................",
		"#..................#",
		"#..................#",
		"#.....#......#.....#",
		"#.....#......#.....#",
        "####..##############"
    };

	largeur = 20;
	hauteur = 15;

	terrain = new Case[largeur*hauteur];

	for(y=0;y<hauteur;++y)
		for(x=0;x<largeur;++x)
            if (terrain_defaut[y][x]=='#')
                terrain[y*largeur+x] = MUR;
            else
                terrain[y*largeur+x] = VIDE;

    //Initialize the snake:
    dirSnake = DROITE;
    int longueurSerpent = 5;
    snake.clear();

    Position posTete;
    posTete.x = 15;
    posTete.y = 8;
	for (int i=0; i<longueurSerpent; i++)
    {
        snake.push_back(posTete);
        posTete.x--;
    }

    // Initialize the fruit
    Position posFruite;
    do{
        posFruite.x = rand()%largeur;
        posFruite.y = rand()%hauteur;
    }while(!posValide(posFruite));

    fruite = posFruite;

    return true;
}

//Evolution of the game: Base on the change of the position of the snake. We
void Jeu::evolue()
{
	list<Position>::iterator itSnake;

    //
    Position posTest = seBalader();

    if(posValide(posTest))
    {
        snake.pop_back();
        snake.push_front(posTest);
    }
    //else : collision happens
    else
    {
        cout << "Collision!" << endl;
        //this->init();
    }

    // If the snake eats the fruit:
    if(fruite == snake.front())
    {
        // - remove the fruit and display it to the next position
        Position *newPosFruite = new Position;
        *newPosFruite = genererRandomPosFruite();
        fruite = *newPosFruite;

        // - generate a random index number for the new fruit pixmap
        nbRandomFruite = rand()%4;

        // - increase the size of the snake by one
        grandirSnake();

        delete newPosFruite;
    }

    // Collision detect
    //collision();
}

int Jeu::getNbCasesX() const
{
    return largeur;
}

int Jeu::getNbCasesY() const
{
    return hauteur;
}

Case Jeu::getCase(const Position &pos) const
{
    assert(pos.x>=0 && pos.x<largeur && pos.y>=0 && pos.y<hauteur);
    return terrain[pos.y*largeur+pos.x];
}

const list<Position> &Jeu::getSnake() const
{
    return snake;
}

bool Jeu::posValide(const Position &pos) const
{
    if (pos.x>=0 && pos.x<largeur && pos.y>=0 && pos.y<hauteur
        && terrain[pos.y*largeur+pos.x]==VIDE)
    {
        list<Position>::const_iterator itSnake;
        itSnake = snake.begin();
        while (itSnake!=snake.end() && *itSnake!=pos)
            itSnake++;
        return (itSnake==snake.end());
    }
    else
        return false;
}

void Jeu::setDirection(Direction dir)
{
    dirSnake = dir;
}

const Direction Jeu::getDirection() const
{
    return dirSnake;
}

void Jeu::ajoutMur()
{
    Position posMur;

    // Trouve une case libre
    do {
        posMur.x = rand()%largeur;
        posMur.y = rand()%hauteur;
    } while (!posValide(posMur));
    terrain[posMur.y*largeur+posMur.x]=MUR;
}

void Jeu::suppressionMur()
{
    Position posMur;

    // Trouve un mur
    do {
        posMur.x = rand()%largeur;
        posMur.y = rand()%hauteur;
    } while (terrain[posMur.y*largeur+posMur.x]!=MUR);
    terrain[posMur.y*largeur+posMur.x]=VIDE;
}


const Position &Jeu::getFruite() const
{
    return fruite;
}


Position Jeu::genererRandomPosFruite()
{
    Position current_pos = fruite;
    Position posFruite;

    do{
        posFruite.x = rand()%largeur;
        posFruite.y = rand()%hauteur;

    }while(!posValide(posFruite) || posFruite==current_pos);
    return posFruite;
}

void Jeu::grandirSnake()
{
    Position* posQueue = new Position;
    *posQueue = snake.back();

    int depX[] = {-1, 1, 0, 0};
    int depY[] = {0, 0, -1, 1};

    // Adding the tail of the snake base on the inverse of the movement direction
    posQueue->x -= depX[dirSnake];
    posQueue->y -= depY[dirSnake];

    snake.push_back(*posQueue);
    delete posQueue;
}
/*
bool Jeu::collision()
{
    // Initialize direction-depended variables
    int depX[] = {-1, 1, 0, 0};
    int depY[] = {0, 0, -1, 1};

    Position* posProchaine = new Position;
    posProchaine->x = snake.front().x + depX[dirSnake];
    posProchaine->y = snake.front().y + depY[dirSnake];

    if(!posValide(*posProchaine)){
        delete posProchaine;
        return true;
    }

    delete posProchaine;
    return false;
}
*/

void Jeu::collision(){
    ;
}

// Function seBalader: to implement the movement of the snake in the game zone:
// - if the snake is out of the game zone, set its head to other side of the map.
// - if not than move the head base on the direction of mouvement
Position Jeu::seBalader()
{
    Position posTete;
    int depX[] = {-1, 1, 0, 0};
    int depY[] = {0, 0, -1, 1};

    // If the snake is moving out of the game zone:
    if(snake.front().x==0 && dirSnake==0){
        posTete.x = largeur-1;
        posTete.y = snake.front().y;
    }
    else if(snake.front().x==largeur-1 && dirSnake==1){
        posTete.x = 0;
        posTete.y = snake.front().y;
    }
    else if(snake.front().y==0 && dirSnake==2){
        posTete.x = snake.front().x;
        posTete.y = hauteur-1;
    }
    else if(snake.front().y==hauteur-1 && dirSnake==3){
        posTete.x = snake.front().x;
        posTete.y = 0;
    }
    // Else if the snake is still in the game zone, move base on the direction of the snake.
    else{
        posTete.x = snake.front().x + depX[dirSnake];
        posTete.y = snake.front().y + depY[dirSnake];
    }
    return posTete;
}

