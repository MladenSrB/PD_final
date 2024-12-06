/* Version: 06.1520 */
#ifndef MAZE_H
#define MAZE_H

#include "redistributable.h"
#include "item.h"
#include "character.h"
#include "redLight.h"
#include "dino_game.h"
#include "clickgame.h"

char getch(); // 非阻塞鍵盤輸入

enum HintType
{
    HINT_SHOP, // 商人
    HINT_COIN, // 金幣
    HINT_MINE  // 地雷
};

class Game
{
private:
    const int SIZE;
    vector<vector<int>> maze;
    map<pair<int, int>, HintType> hints;
    int playerX, playerY;

    int dx[4];
    int dy[4];
    Player &player;
    bool haveKey = false; // 有鑰匙才能出來

public:
    Game(Player &p, bool &key);
    void generateMaze(int x, int y);
    void placeHints();
    void displayMaze();
    void movePlayer(char move);
    void handleHint();
    void start();
};

#endif