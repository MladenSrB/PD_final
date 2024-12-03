#include "redistributable.h"

using namespace std;
using namespace chrono;



class DinoGame
{
protected:
  static const int width = 50;
  static const int height = 10;
  static const char ground;
  static const char dinosaur;
  static const char obstacle;

  int dinoY;
  bool isJumping;
  bool gameOver;

public:
  DinoGame() : dinoY(height - 2), isJumping(false), gameOver(false) {}

  void setNonBlockingInput();
  void resetInput();
  void displayGame(char map[height][width], int score);
  void updateMap(char map[height][width], vector<int> &obstacleX, int &score, int &obstacleSpawnTimer);
  void jump();
  void runGame();
};

static const int width = 50;
static const int height = 10;
static const char ground = '_';
static const char dinosaur = 'T';
static const char obstacle = 'O';