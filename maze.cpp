/* Version: 06.1700 */
#include "maze.h"
using namespace std;


void clearAndResetCursor()
{
    system("clear");
    std::cout << "\033[H"; // move cursor to top of the screen
}

void setTerminalSize(int rows, int cols)
{
    std::string command = "printf '\\e[8;" + std::to_string(rows) + ";" + std::to_string(cols) + "t'";
    system(command.c_str());
}

// 用來設置非阻塞鍵盤輸入
char getch()
{
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt); // 獲取當前終端設置printCentered("使用方向鍵選擇，Enter 確認使用", screenWidth)
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);        // 設置為非終端模式 (不等到Enter)
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); // 設置終端模式
    ch = getchar();                          // 讀取一個字符
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // 恢復終端設置
    return ch;
}

// 構造函數
Game::Game(Player &p, bool &key) : SIZE(21), maze(SIZE, vector<int>(SIZE, 0)), playerX(1), playerY(1), player(p), haveKey(key)
{
    dx[0] = -1;
    dx[1] = 1;
    dx[2] = 0;
    dx[3] = 0;
    dy[0] = 0;
    dy[1] = 0;
    dy[2] = -1;
    dy[3] = 1;

    srand(time(0));
    generateMaze(1, 1);
    maze[1][1] = 1;               // 起點 (1, 1)
    maze[SIZE - 2][SIZE - 2] = 1; // 終點 (SIZE-2, SIZE-1)

    // 保證四周的牆壁
    for (int i = 0; i < SIZE; ++i)
    {
        maze[0][i] = maze[SIZE - 1][i] = 0;
        maze[i][0] = maze[i][SIZE - 1] = 0;
    }
    maze[SIZE - 2][SIZE - 1] = 1; // 開洞
    // 隨機放置提示標示
    placeHints();
}

// 生成迷宮的深度優選搜尋法
void Game::generateMaze(int x, int y)
{
    maze[x][y] = 1; // 標記為通道

    // 隨機打亂方向順序
    for (int i = 0; i < 4; ++i)
    {
        int r = rand() % 4;
        swap(dx[i], dx[r]);
        swap(dy[i], dy[r]);
    }

    // 按隨機方向挖掘
    for (int i = 0; i < 4; ++i)
    {
        int nx = x + dx[i] * 2;
        int ny = y + dy[i] * 2;

        // 檢查邊界和是否已訪問
        if (nx > 0 && nx < SIZE - 1 && ny > 0 && ny < SIZE - 1 && maze[nx][ny] == 0)
        {
            maze[(x + nx) / 2][(y + ny) / 2] = 1; // 打通牆壁
            generateMaze(nx, ny);
        }
    }
}

// 隨機放置提示標示
void Game::placeHints()
{
    // 放置商人
    int shopCount = rand() % 5 + 5;
    for (int i = 0; i < shopCount; i++)
    {
        while (true)
        {
            int x = rand() % (SIZE - 2) + 1;
            int y = rand() % (SIZE - 2) + 1;
            if (maze[x][y] == 1 && hints.find({x, y}) == hints.end() && (x != playerX || y != playerY))
            {
                hints[make_pair(x, y)] = HINT_SHOP; // 商人
                break;
            }
        }
    }

    // 放置金幣
    int coinCount = rand() % 5 + 5;
    for (int i = 0; i < coinCount; i++)
    {
        while (true)
        {
            int x = rand() % (SIZE - 2) + 1;
            int y = rand() % (SIZE - 2) + 1;
            if (maze[x][y] == 1 && hints.find({x, y}) == hints.end() && (x != playerX || y != playerY))
            {
                hints[make_pair(x, y)] = HINT_COIN; // 金幣
                break;
            }
        }
    }

    // 放置遊戲
    int gameCount = rand() % 5 + 5;
    for (int i = 0; i < gameCount; i++)
    {
        while (true)
        {
            int x = rand() % (SIZE - 2) + 1;
            int y = rand() % (SIZE - 2) + 1;
            if (maze[x][y] == 1 && hints.find({x, y}) == hints.end() && (x != playerX || y != playerY))
            {
                hints[make_pair(x, y)] = HINT_MINE; // 地雷
                break;
            }
        }
    }
}

// 顯示迷宮和玩家資訊
void Game::displayMaze()
{
    setTerminalSize(30, 50);
    clearAndResetCursor();
    cout << "Level: " << player.getlevel() << " | EXP: " << player.getExp() << " | HP: " << player.gethp()
         << " | ATK: " << player.getatk() << " | Coin: " << player.getCoin();
    if (haveKey) { cout << " | ⚿";}
    cout << "\n\n";

    // 如果玩家的盾牌正在啟用，顯示盾牌剩餘時間
    int shieldTime = player.getShieldRemainingTime();
    if (shieldTime > 0)
    {
        cout << "使用盾牌，保護時效內移動將不會損耗任何HP，效果將持續 " << shieldTime << " 秒！" << endl;
    }

    for (int i = 0; i < SIZE; ++i)
    {
        for (int j = 0; j < SIZE; ++j)
        {
            if (i == playerX && j == playerY)
            {
                cout << "P "; // 玩家位置
            }
            else if (hints.find({i, j}) != hints.end())
            {
                // 根據提示類型顯示不同符號
                switch (hints[{i, j}])
                {
                case HINT_SHOP:
                    cout << "𖠋 "; // 商人
                    break;
                case HINT_COIN:
                    cout << "$ "; // 金幣
                    break;
                case HINT_MINE:
                    cout << "𓉸 "; // 進入遊戲入口
                    break;
                }
            }
            else if (maze[i][j] == 1)
            {
                cout << "  "; // 通道
            }
            else
            {
                cout << "██"; // 牆壁
            }
        }
        cout << "\n";
    }
}

// 玩家移動控制
void Game::movePlayer(char move)
{
    int newX = playerX, newY = playerY;

    if (move == 'w' && playerX > 0 && maze[playerX - 1][playerY] == 1)
    {
        newX = playerX - 1;
        player.move();
    }
    else if (move == 's' && playerX < SIZE - 1 && maze[playerX + 1][playerY] == 1)
    {
        newX = playerX + 1;
        player.move();
    }
    else if (move == 'a' && playerY > 0 && maze[playerX][playerY - 1] == 1)
    {
        newY = playerY - 1;
        player.move();
    }
    else if (move == 'd' && playerY < SIZE - 1 && maze[playerX][playerY + 1] == 1)
    {
        newY = playerY + 1;
        player.move();
    }
    else if (move == 'b')
    {
        clearAndResetCursor();
        player.openBackpack();
    }

    playerX = newX;
    playerY = newY;
    
}

// 處理提示事件
void Game::handleHint()
{
    if (hints.count({playerX, playerY}) > 0)
    {
        Merchant *merchant = nullptr;
        int coinAmount = rand() % 50 + 1;

        HintType type = hints[{playerX, playerY}];
        switch (type)
        {
        case HINT_SHOP:
            merchant = new Merchant();
            cout << "\n你遇到了一位商人！以下是他的商品：\n";
            merchant->printAllGoods();
            cout << "[4] key: $ 50\n\n輸入商品編號購買，或輸入 esc 離開：";

            try

            {
                char input = getch();
                if (input >= '0' && input < '4')
                {
                    Item *purchasedItem = merchant->sellGood(input - '0');
                    player.boughtItem(purchasedItem);
                    break;
                }
                else if (input == '4')
                {
                    if (player.getCoin() >= 50)
                    {
                        player.decreaseCoin(50);
                        cout << "購買成功！\n";
                        haveKey = true;
                        break;
                    }
                    else
                    {
                        cout << "餘額不足\n";
                        break;
                    }
                }
                else if (input == '\033')
                {
                    break;
                }
                else
                {
                    __throw_out_of_range("Invalid Index");
                }
            }
            catch (std::exception &e)
            {
                cout << "無效的輸入值：" << e.what() << "\n";
            }

        case HINT_COIN:
            player.addCoin(coinAmount);
            cout << "\n哎呦真幸運，恭喜獲得 " << coinAmount << " 金幣！\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
            break;
        case HINT_MINE:
            cout << "\n看！前方出現一個神秘入口，進入看看會有驚喜在等你呦！\n";
            std::this_thread::sleep_for(std::chrono::seconds(2));
            int gameType = rand() % 3;
            if (gameType == 0)
            {
                RedLightGame redLight(10, 100);
                redLight.startGame();
            }
            else if (gameType == 1)
            {
                Clickgame clickGame;
                clickGame.startGame(player);
            }
            else
            {
                DinoGame dinoGame;
                dinoGame.startGame(player);
            }
            player.addExp(50);
            break;
        }

        // 移除已觸發的提示
        hints.erase({playerX, playerY});
    }
}

// 主遊戲循環
void Game::start()
{
    while (true)
    {
        clearAndResetCursor();
        displayMaze();

        // 玩家移動
        char move = getch(); // 使用 getch() 讀取即時鍵盤輸入
        movePlayer(move);

        // 處理提示事件
        handleHint();

        if (player.gethp() <= 1)
        { // 確認玩家血量
            clearAndResetCursor();
            std::cout << "生命即將結束！是否需要查看背包裡有哪些道具可以使用嗎？\n";
            std::cout << "請輸入 Y 或 N：";
            char input = getch();
            while (input)
            {
                char input = getch();
                if (input == 'y' || 'y')
                {
                    clearAndResetCursor();
                    player.openBackpack();
                    if (player.gethp() <= 0)
                    {
                        clearAndResetCursor();
                        std::cout << "Game Over!" << endl;
                        exit(0);
                    }
                    else
                    {
                        break;
                    }
                }
                else if (input == 'N' || 'n')
                {
                    clearAndResetCursor();
                    std::cout << "Game Over!" << endl;
                    exit(0);
                }
                else
                {
                    input = getch();
                }
            }
        }

        // 檢查玩家是否到達終點
        if (playerX == SIZE - 2 && playerY == SIZE - 1)
        {
            if (haveKey)
            {
                clearAndResetCursor();
                displayMaze();
                cout << "\nCongratulations! You win!\n";
                break;
            }
            else
            {
                cout << "沒有鑰匙，無法開啟迷宮大門！請返回賺取更多金幣購買鑰匙！\n";
                std::this_thread::sleep_for(std::chrono::seconds(2));
            }
        }
    }
}

int main()
{
    clearAndResetCursor();
    cout << "歡迎來到迷宮遊戲！\n";
    cout << "請輸入你的角色名字: ";
    string playerName;
    cin >> playerName;

    // 創建玩家角色
    Player player(playerName);
    cout << "\n你好, " << playerName << "！祝你好運！\n";
    bool haveKey = false;

    while (true)
    {
        clearAndResetCursor();
        // 主菜單
        cout << "==============================" << endl;
        cout << "        迷宮遊戲主菜單        " << endl;
        cout << "==============================" << endl;
        cout << "1. 開始遊戲" << endl;
        cout << "2. 遊戲說明" << endl;
        cout << "3. 離開遊戲" << endl;
        cout << "請選擇 (輸入 1, 2 或 3): ";

        char input = getch();
        while (input)
        {
            char input = getch();
            if (input == '1')
            {
                // 開始遊戲
                cout << "正在進入遊戲...\n";
                Game game(player, haveKey); // 傳遞玩家角色
                game.start();               // 開始遊戲
            }
            else if (input == '2')
            {
                // 遊戲說明
                clearAndResetCursor();
                // 檔案名稱
                string filename = "./gameIntro.txt";
                
                // 開啟檔案
                ifstream file(filename);
                
                // 確認檔案是否成功開啟
                if (!file.is_open()) {
                    std::cerr << "無法開啟檔案: " << filename << std::endl;
                    return 1; // 返回錯誤代碼
                }

                // 逐行讀取並印出
                string line;
                while (getline(file, line)) {
                    cout << line << std::endl;
                }

                // 關閉檔案
                file.close();
                while (getch() != '\n')
                {
                }
                break;
            }
            else if (input == '3')
            {
                clearAndResetCursor();
                cout << "再見！" << playerName << endl;
                exit(0);
            }
            else
            {
                input = getch();
            }
        }
    }
    return 0;
}
