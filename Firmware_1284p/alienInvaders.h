/*
* main.c
*
* Created: 2015/04/01
*  Author: Timothy Thompkins
*/

extern unsigned long max_res; // Need to check what the max value of these will be.


typedef struct OS
{
  	char rasberry_boolean;

} OS;

typedef struct Game
{
  	long int score;
  	int lives;
    unsigned int gameMap[max_res][max_res]; // This is the resolution of the game maybe use PROGMEM to modularize

} Game;

typedef struct Player
{
		uint8_t shield_level, state;
		uint8_t health_level;
		int x_pos;

} Player;

typedef struct Bullet
{
		int x_pos, y_pos;

} Bullet;

typedef struct Alien
{
		uint8_t health_level;
		int x_pos, y_pos;

} Alien;


/* Function prototypes */
char getOS                (void); // Depending on the OS, the python program should deal with resizing the window (resolution stays same)
void setGameMap           (char, unsigned long); // Call this before creating the game, we need to create the game map

void gameInit	   			    (void);
void removeLife           (void);
void gameOver             (void);

int getCurrentPlayerPos   (int); // This will be given over serial

void alienGroupInit       (void);
void playerInit           (void);

void bulletPlace          (void);
void shieldPlace          (void);

void movePlayer           (int last_x_pos);
void moveBullet           (int last_y_pos);
void moveAlienGroup       (int last_x_pos, int last_y_pos);
void checkCollision       (int, int, int, int); // This will call remove life. It takes the x and y positions of any 2 objects.

void updateGame           (void);
