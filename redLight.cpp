/* Version: 04.1517 */
#include "redLight.h"
using namespace std;

// set terminal to non blocking mode
void RedLightGame::setNonBlockingInput(bool enable)
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

void RedLightGame::startGame()
{
    bool isSucceed = false;

    // initialize game
    int playerPos = PLAYER_START_POS;
    bool isRedLight = false;
    float redLightSwitchTime = 0; // record light switching time
    int timeLeft = TIME_LIMIT;

    std::srand(static_cast<unsigned>(std::time(0)));

    auto startTime = std::chrono::steady_clock::now();
    auto redLightTime = startTime;

    setNonBlockingInput(true); // enable terminal non blocking mode

    while (timeLeft > 0)
    {
        // calculate time left
        auto now = std::chrono::steady_clock::now();
        timeLeft = TIME_LIMIT - static_cast<int>(
                                    std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count());

        // switch light randomly
        if (std::rand() % 100 < 5)
        { // 5% 機率每迴圈切換燈號
            isRedLight = !isRedLight;
            redLightTime = now;
        }

        // 清除螢幕
        system("clear");

        // 顯示遊戲狀態
        std::cout << "Time Left: " << timeLeft << "s\n";
        std::cout << (isRedLight ? "RED LIGHT" : "GREEN LIGHT") << "\n";

        // 顯示遊戲場景
        for (int i = 0; i < WINDOW_WIDTH; ++i)
        {
            if (i == playerPos)
            {
                std::cout << "P"; // 玩家
            }
            else if (i == FINISH_LINE_X)
            {
                std::cout << "|"; // 終點線
            }
            else
            {
                std::cout << "-";
            }
        }
        std::cout << "\n";

        // 檢查玩家是否到達終點
        if (playerPos >= FINISH_LINE_X)
        {
            std::cout << "Phew! You made it!\n";
            isSucceed = true;
            break;
        }

        // 處理玩家輸入
        char input;
        if (read(STDIN_FILENO, &input, 1) > 0)
        { // 非阻塞讀取輸入
            if (input == 'a' || input == 'A')
            {
                playerPos = std::max(playerPos - 1, 0); // 向左移動
            }
            else if (input == 'd' || input == 'D')
            {
                // 檢查是否違反紅燈規則
                float timeSinceRed = std::chrono::duration_cast<std::chrono::milliseconds>(now - redLightTime).count() / 1000.0;
                if (isRedLight && timeSinceRed > BUFFER_TIME)
                {
                    std::cout << "You moved during RED LIGHT! Game Over!\n";
                    break;
                }
                playerPos = std::min(playerPos + 1, FINISH_LINE_X); // 向右移動
            }
        }

        // 暫停片刻模擬遊戲節奏
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    if (timeLeft <= 0)
    {
        std::cout << "Times up! you lost!";
    }

    if (isSucceed)
    {
        std::cout << "You've gained " << gainedEXP << " EXP!" << '\n';
    }
    else
    {
        std::cout << "You've lost " << lostHP << " pts of HP!" << '\n';
    }

    std::cout << "Press \"Enter\" to continue\r" << endl;
    std::cout.flush();
    setNonBlockingInput(false); // 恢復終端默認模式
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    for (int sec = 1; sec > 0; sec--)
    {
        std::cout << "Returning to maze in " << sec << " seconds...\r";
        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "                                         \r";
        std::cout.flush();
    }
}