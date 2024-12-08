/* Version: 06.1746 */
#ifndef REDLIGHT
#define REDLIGHT

#include "redistributable.h"
#include "character.h"

const int WINDOW_WIDTH = 30;    // 終端寬度 (用於顯示遊戲)
const int PLAYER_START_POS = 1; // 玩家起始位置
const int FINISH_LINE_X = 30;   // 終點位置
const int TIME_LIMIT = 30;      // 時間限制 (秒)
const float BUFFER_TIME = 0.4;  // 紅燈緩衝時間

class RedLightGame
{
private:
    const int lostHP;
    const int gainedEXP;
    const int gainedCoin;

public:
    RedLightGame(int HP, int EXP, int coin): lostHP(HP), gainedEXP(EXP), gainedCoin(gainedCoin){};
    void startGame(Player& player);
    void setNonBlockingInput(bool enable);
};

#endif
