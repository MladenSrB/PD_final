#include "redistributable.h"
using namespace std;

// 用來設置非阻塞鍵盤輸入
char getch() {
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt); // 獲取當前終端設置
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // 設置為非終端模式 (不等到Enter)
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); // 設置終端模式
    ch = getchar(); // 讀取一個字符
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // 恢復終端設置
    return ch;
}

class Game {
private:
    const int SIZE; // 迷宮大小
    vector<vector<int> > maze; // 迷宮
    set<pair<int, int> > hints; // 存放提示標示的位置
    int playerX, playerY; // 玩家起始位置

    int dx[4]; // 定義方向：上下左右
    int dy[4];

public:
    Game() : SIZE(21), maze(SIZE, vector<int>(SIZE, 0)), playerX(1), playerY(1) {
        dx[0] = -1; dx[1] = 1; dx[2] = 0; dx[3] = 0;
        dy[0] = 0; dy[1] = 0; dy[2] = -1; dy[3] = 1;

        srand(time(0));
        generateMaze(1, 1);
        maze[1][1] = 1; // 起點 (1, 1)
        maze[SIZE - 2][SIZE - 2] = 1; // 終點 (SIZE-2, SIZE-1)

        // 保證四周的牆壁
        for (int i = 0; i < SIZE; ++i) {
            maze[0][i] = maze[SIZE - 1][i] = 0;
            maze[i][0] = maze[i][SIZE - 1] = 0;
        }

        // 隨機放置提示標示
        placeHints(5); // 放置 5 個提示標示
    }

    // 生成迷宮的深度優選搜尋法
    void generateMaze(int x, int y) {
        maze[x][y] = 1; // 標記為通道

        // 隨機打亂方向順序
        for (int i = 0; i < 4; ++i) {
            int r = rand() % 4;
            swap(dx[i], dx[r]);
            swap(dy[i], dy[r]);
        }

        // 按隨機方向挖掘
        for (int i = 0; i < 4; ++i) {
            int nx = x + dx[i] * 2;
            int ny = y + dy[i] * 2;

            // 檢查邊界和是否已訪問
            if (nx > 0 && nx < SIZE - 1 && ny > 0 && ny < SIZE - 1 && maze[nx][ny] == 0) {
                maze[(x + nx) / 2][(y + ny) / 2] = 1; // 打通牆壁
                generateMaze(nx, ny);
            }
        }
    }

    // 隨機放置提示標示
    void placeHints(int hintCount) {
        while (hints.size() < hintCount) {
            int x = rand() % (SIZE - 2) + 1;
            int y = rand() % (SIZE - 2) + 1;
            if (maze[x][y] == 1 && (x != playerX || y != playerY)) {
                hints.insert(make_pair(x, y)); // 使用 std::make_pair
            }
        }
    }

    // 顯示迷宮和玩家的位置
    void displayMaze() {
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                if (i == playerX && j == playerY) {
                    cout << "P "; // 玩家位置
                } else if (hints.count(make_pair(i, j)) > 0) { // 使用 std::make_pair
                    cout << "! "; // 提示標示
                } else if (maze[i][j] == 1) {
                    cout << "  "; // 通道
                } else {
                    cout << "██"; // 牆壁
                }
            }
            cout << "\n";
        }
    }

    // 玩家移動控制
    void movePlayer(char move) {
        int newX = playerX, newY = playerY;

        if (move == 'w' && playerX > 0 && maze[playerX - 1][playerY] == 1) {
            newX = playerX - 1;
        } else if (move == 's' && playerX < SIZE - 1 && maze[playerX + 1][playerY] == 1) {
            newX = playerX + 1;
        } else if (move == 'a' && playerY > 0 && maze[playerX][playerY - 1] == 1) {
            newY = playerY - 1;
        } else if (move == 'd' && playerY < SIZE - 1 && maze[playerX][playerY + 1] == 1) {
            newY = playerY + 1;
        }

        playerX = newX;
        playerY = newY;
    }

    // 處理提示事件
    void handleHint() {
        if (hints.count(make_pair(playerX, playerY)) > 0) { // 使用 std::make_pair
            cout << "\n嘿嘿，你觸發了一個提示！\n";
            cout << "請輸入 'ok' 繼續...\n";
            string input;
            do {
                cin >> input;
            } while (input != "ok");
            hints.erase(make_pair(playerX, playerY)); // 移除提示標示
        }
    }

    // 主遊戲循環
    void start() {
        while (true) {
            system("clear"); // 在 macOS 上使用 clear 清除屏幕
            displayMaze();
            cout << "\nUse WASD to move (w = up, s = down, a = left, d = right)\n";
            cout << "終點為最右下方\n";

            // 玩家移動
            char move = getch(); // 使用 getch() 讀取即時鍵盤輸入
            movePlayer(move);

            // 處理提示事件
            handleHint();

            // 檢查玩家是否到達終點
            if (playerX == SIZE - 2 && playerY == SIZE - 2) {
                system("clear");
                displayMaze();
                cout << "\nCongratulations! You win!\n";
                break;
            }
        }
    }
};

int main() {
    Game game; // 創建遊戲對象
    game.start(); // 開始遊戲
    return 0;
}
