/* Version: 03.2001 */
#include "maze.h"
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

// 構造函數
Game::Game(Player &p) : SIZE(21), maze(SIZE, vector<int>(SIZE, 0)), playerX(1), playerY(1), player(p) {
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
    placeHints(10); // 放置 5 個提示標示
}

// 生成迷宮的深度優選搜尋法
void Game::generateMaze(int x, int y) {
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
void Game::placeHints(int hintCount) {
    while (hints.size() < hintCount) {
        int x = rand() % (SIZE - 2) + 1;
        int y = rand() % (SIZE - 2) + 1;
        if (maze[x][y] == 1 && (x != playerX || y != playerY)) {
            hints.insert(make_pair(x, y)); // 使用 std::make_pair
        }
    }
}

// 顯示迷宮和玩家資訊
void Game::displayMaze() {
    //cout << "   經驗: " << player.getexp()
      //   << "   血量: " << player.gethp() << "\n\n";

    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (i == playerX && j == playerY) {
                cout << "P ";
            } else if (hints.count(make_pair(i, j)) > 0) {
                cout << "! ";
            } else if (maze[i][j] == 1) {
                cout << "  ";
            } else {
                cout << "██";
            }
        }
        cout << "\n";
    }
}


// 玩家移動控制
void Game::movePlayer(char move) {
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
void Game::handleHint() {
    if (hints.count(make_pair(playerX, playerY)) > 0) {
        cout << "\n你觸發了提示點！準備挑戰紅綠燈遊戲！\n";
        int gameType = rand() % 3;
        if (gameType == 0){
            RedLightGame redLight(10, 100);
            redLight.startGame();
        }
        else if (gameType == 1){
            RedLightGame redLight(10, 100);
            redLight.startGame();
            /*DinoGame dino;
            dino.runGame(player);*/
        }
        else {
            /*clickgame click;
            click.playGame(player);*/
            RedLightGame redLight(10, 100);
            redLight.startGame();
        }
        
        player.addExp(50);
        hints.erase(make_pair(playerX, playerY));
    }
}

// 主遊戲循環
void Game::start() {
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

int main() {
    cout << "歡迎來到迷宮遊戲！\n";
    cout << "請輸入你的角色名字: ";
    string playerName;
    cin >> playerName;

    // 創建玩家角色
    Player player(playerName);
    cout << "\n你好, " << playerName << "！祝你好運！\n";

    while (true) {
        // 主菜單
        cout << "==============================" << endl;
        cout << "        迷宮遊戲主菜單        " << endl;
        cout << "==============================" << endl;
        cout << "1. 開始遊戲" << endl;
        cout << "2. 遊戲說明" << endl;
        cout << "3. 離開遊戲" << endl;
        cout << "請選擇 (輸入 1, 2 或 3): ";
        
        int choice;
        cin >> choice;
        cout << endl;

        if (choice == 1) {
            // 開始遊戲
            cout << "正在進入遊戲...\n";
            Game game(player); // 傳遞玩家角色
            game.start();      // 開始遊戲
        } else if (choice == 2) {
            // 遊戲說明
            cout << "遊戲說明：" << endl;
            cout << "1. 使用鍵盤 WASD 來控制角色移動。" << endl;
            cout << "2. 迷宮中隨機生成提示標誌 (!)，" << endl;
            cout << "   觸碰它們會觸發小遊戲，例如紅綠燈遊戲。" << endl;
            cout << "3. 找到迷宮的終點來完成挑戰！" << endl;
            cout << "4. 每完成挑戰，你將獲得經驗值 (EXP) 或失去血量 (HP)。" << endl;
            cout << endl;
        } else if (choice == 3) {
            // 離開遊戲
            cout << "感謝遊玩！再見！" << endl;
            break;
        } else {
            // 無效選項
            cout << "無效選項，請重新輸入！" << endl;
        }

        // 暫停等待用戶繼續
        cout << "\n按 Enter 鍵返回主選單..." << endl;
        cin.ignore();
        cin.get();
    }

    return 0;
}
