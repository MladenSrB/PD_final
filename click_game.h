#include "redistributable.h"

using namespace std;
using namespace chrono;

class clickgame
{
private:
    //bool hasRevive = true;
    int monsterHP = 30;
    int totalClicks = 0;
    int roundClicks = 0;
    int currentVisibleLines;
public:
    void setNonBlockingInput(bool enable);
    bool isKeyPressed();
    char getKey();
    void clearScreen();
    char getLastInput();
    void printMonster(const vector<string>& monster, int visibleLines);
    void playGame();
};
