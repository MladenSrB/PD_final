/* Version: 03.1846 */
#ifndef CLICKGAME
#define CLICKGAME

#include "redistributable.h"
#include "character.h"
#include "item.h"

using namespace std;
using namespace chrono;

class clickgame
{
private:
    int monsterHP;
    int totalClicks;
    int roundClicks;
    int currentVisibleLines;
    int monsterATK;
    int monsterlv;
    vector<string> monster; // 怪物圖案

public:
    clickgame();
    void setNonBlockingInput(bool enable);
    bool isKeyPressed();
    char getKey();
    void clearScreen();
    char getLastInput();
    void printMonster(int visibleLines);
    void playGame(Player &name);
};

#endif