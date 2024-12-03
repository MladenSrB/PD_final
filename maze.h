/* Version: 03.2001 */
#ifndef MAZE_H
#define MAZE_H

#include "redistributable.h"
#include "character.h"
#include "redLight.h"
#include "item.h"
#include "dino_game.h"
#include "clickgame.h"

char getch(); // 非阻塞鍵盤輸入

class Game {
private:
    const int SIZE;
    vector<vector<int> > maze;
    set<pair<int, int> > hints;
    int playerX, playerY;

    int dx[4]; // 移動方向
    int dy[4];
    Player &player; // 傳入玩家物件

public:
    Game(Player &p); // 傳入 Player 參考
    void generateMaze(int x, int y);
    void placeHints(int hintCount);
    void displayMaze();
    void movePlayer(char move);
    void handleHint();
    void start();
};

#endif