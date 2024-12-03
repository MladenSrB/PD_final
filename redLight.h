#ifndef REDLIGHT
#define REDLIGHT

const int WINDOW_WIDTH = 80;    // 終端寬度 (用於顯示遊戲)
const int PLAYER_START_POS = 1; // 玩家起始位置
const int FINISH_LINE_X = 70;   // 終點位置
const int TIME_LIMIT = 10;      // 時間限制 (秒)
const float BUFFER_TIME = 0.4;  // 紅燈緩衝時間

class RedLightGame
{
private:
    int lostHP = 0;
    int gainedEXP = 0;

public:
    RedLightGame(int HP, int EXP){ lostHP = HP; gainedEXP = EXP; };
    void startGame();
    void setNonBlockingInput(bool enable);
};

#endif
