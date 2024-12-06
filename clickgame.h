/* Version: 06.1520 */
#ifndef CLICKGAME
#define CLICKGAME

#include "redistributable.h"
#include "character.h"
#include "item.h"

using namespace std;
using namespace chrono;

class Clickgame
{
private:
    int monsterHP;
    int totalClicks;
    int roundClicks;
    int currentVisibleLines;
    int monsterATK;
    int monsterlv;
    vector<string> monsterAppearance; // 怪物圖案

public:
    Clickgame();
    void setNonBlockingInput(bool enable);
    bool isKeyPressed();
    char getKey();
    void clearScreen();
    char getLastInput();
    void printMonster(int visibleLines);
    void startGame(Player &name);
};

#endif