/* Version: 06.1746 */
#include "dino_game.h"

using namespace std;
using namespace chrono;

// set terminal to non blocking mode
void DinoGame::setNonBlockingInput(bool enable)
{
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    if (enable)
    {
        tty.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &tty);
        fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) | O_NONBLOCK); // 設置非阻塞模式
    }
    else
    {
        tty.c_lflag |= (ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &tty);
        fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) & ~O_NONBLOCK); // 關閉非阻塞模式
    }
}

void DinoGame::displayGame(char map[height][width], int score, int countdown)
{
    system("clear");
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            cout << map[i][j];
        }
        cout << endl;
    }
    cout << "Score: " << score << endl;
    cout << "Time remaining: " << countdown << " seconds" << endl;
}

void DinoGame::updateMap(char map[height][width], vector<int> &obstacleX, int &score, int &obstacleSpawnTimer)
{
    // 清除上一幀
    for (int i = 0; i < height; ++i)
    {
        fill(map[i], map[i] + width, ' ');
    }

    // 畫地面
    fill(map[height - 1], map[height - 1] + width, '_');

    // 畫恐龍
    map[dinoY][1] = dinosaur;

    // 生成新障礙物
    if (obstacleSpawnTimer <= 0)
    {
        obstacleX.push_back(width - 1);
        obstacleSpawnTimer = rand() % 10 + 3; // 調整這裡，讓間隔時間更短
    }
    else
    {
        obstacleSpawnTimer--;
    }

    // 更新障礙物位置和計分
    for (int &x : obstacleX)
    {
        if (x == 1 && dinoY == height - 2)
        { // 碰撞檢測
            gameOver = true;
        }

        if (x == 1 && dinoY != height - 2)
        { // 成功避開障礙物
            score++;
        }

        x--;
    }

    obstacleX.erase(remove_if(obstacleX.begin(), obstacleX.end(), [](int x)
                              { return x < 0; }),
                    obstacleX.end());

    // 畫障礙物
    for (int x : obstacleX)
    {
        if (x > 0 && x < width)
        {
            map[height - 2][x] = obstacle;
        }
    }
}

void DinoGame::jump()
{
    if (isJumping)
        return;

    isJumping = true;
    for (int i = 0; i < 3; ++i)
    {
        dinoY--;
        this_thread::sleep_for(milliseconds(100));
    }
    this_thread::sleep_for(milliseconds(50));
    for (int i = 0; i < 3; ++i)
    {
        dinoY++;
        this_thread::sleep_for(milliseconds(100));
    }
    isJumping = false;
}

void DinoGame::startGame(Player &player)
{
    char map[height][width];
    vector<int> obstacleX;
    int obstacleSpawnTimer = 0;
    int score = 0;
    int coin = 0;
    int exp = 0;
    int blood = 10;

    srand(time(0));

    setNonBlockingInput(true);

    auto startTime = chrono::steady_clock::now(); // 開始計時

    while (!gameOver)
    {
        // 檢查時間是否超過30秒
        auto currentTime = chrono::steady_clock::now();
        auto elapsedTime = chrono::duration_cast<chrono::seconds>(currentTime - startTime).count();
        int countdown = 15 - elapsedTime;

        if (countdown <= 0)
        {
            coin = score;
            exp = score * 2;
            player.addCoin(coin);
            player.addExp(exp);
            cout << "Congratulations! You won the game!\n"
                 << endl;
            cout << "coin +" << coin << ", total coin: " << player.getCoin() << endl;
            cout << "exp +" << exp << ", total exp: " << player.getExp() << endl;
            break;
        }

        char ch;
        if (read(STDIN_FILENO, &ch, 1) > 0 && ch == ' ')
        {
            thread t([this]
                     { jump(); });
            t.detach();
        }

        updateMap(map, obstacleX, score, obstacleSpawnTimer);
        displayGame(map, score, countdown);
        this_thread::sleep_for(milliseconds(100));
    }

    if (gameOver)
    {
        
        coin = score / 3;
        exp = score / 2;
        player.addCoin(coin);
        player.addExp(exp);
        player.decreaseHp(blood);
        cout << "Game Over!" << endl;
        cout << "coin +" << coin << ", total coin: " << player.getCoin() << endl;
        cout << "exp +" << exp << ", total exp: " << player.getExp() << endl;
        cout << "hp -" << blood << ", total hp: " << player.gethp() << endl;
        
        for (int sec = 1; sec > 0; sec--)
        {
            std::cout << "Returning to maze in " << sec << " seconds...\r";
            std::cout.flush();
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::cout << "                                         \r";
            std::cout.flush();
        }
    }

    setNonBlockingInput(false);
    // 提示返回迷宮
    cout << "Returning to the maze. Press Enter to continue..." << endl;

    char input;
    while (read(STDIN_FILENO, &input, 1) > 0 && input != '\n')
    {
        // 等待用戶按下 Enter 鍵
    }
    cout << "Bye!";
    std::this_thread::sleep_for(std::chrono::seconds(1));
}