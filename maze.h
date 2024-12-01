#include "redistributable.h"
using namespace std;

// 用來設置非阻塞鍵盤輸入
char getch();

class Game {
private:
    const int SIZE; // 迷宮大小
    vector<vector<int>> maze; // 迷宮
    set<pair<int, int>> hints; // 存放提示標示的位置
    int playerX, playerY; // 玩家起始位置

    int dx[4]; // 定義方向：上下左右
    int dy[4];

public:
    Game(); // 構造函數
    void generateMaze(int x, int y); // 生成迷宮
    void placeHints(int hintCount); // 放置提示標示
    void displayMaze(); // 顯示迷宮
    void movePlayer(char move); // 玩家移動
    void handleHint(); // 處理提示事件
    void start(); // 主遊戲循環
};