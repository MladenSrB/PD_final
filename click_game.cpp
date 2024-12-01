#include "redistributable.h"

using namespace std;
using namespace chrono;
//test

class click_game
{
private:
    bool hasRevive;
    int monsterHP;
    int totalClicks;
    int roundClicks;
    int currentVisibleLines;
    const int maxHP = 30;
    const int durationInSeconds = 10;

    vector<string> monster = {
        "   (o_o)   ",
        "  <|   |>  ",
        "   |   |   ",
        "  /|   |\\  ",
        " /_|   |_\\ ",
    };

    // 跨平台按键检测函数
    char getch()
    {
        struct termios oldt, newt;
        char ch;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
    }

    void clearScreen()
    {
        for (int i = 0; i < 50; ++i)
        {
            cout << endl;
        }
    }

    char getLastInput()
    {
        string input;
        char lastValidChar = '\0';
        getline(cin, input);
        for (char c : input)
        {
            if (!isspace(c))
            {
                lastValidChar = c;
            }
        }
        return tolower(lastValidChar);
    }

    void printMonster()
    {
        cout << "\n怪物狀態：" << endl;
        for (int i = 0; i < currentVisibleLines; i++)
        {
            cout << monster[i] << endl;
        }
        cout << endl;
    }

public:
    click_game() : hasRevive(true), monsterHP(30), totalClicks(0), roundClicks(0), currentVisibleLines(monster.size()) {}

    void playGame()
    {
        srand(static_cast<unsigned int>(time(0)));

        cout << "===== 鍵盤連點遊戲 =====" << endl;
        cout << "一隻怪物出現了！" << endl;
        printMonster();
        cout << "=== 規則 ===" << endl;
        cout << "在限定時間內快速點擊Enter 鍵來攻擊怪物！" << endl;
        cout << "每點擊一次Enter 鍵即可減少怪物 1 HP" << endl;
        cout << "這隻怪物總共有 30 HP！" << endl;
        cout << "你有 " << durationInSeconds << " 秒的時間擊敗它！" << endl;
        cout << "按下 Enter 鍵開始..." << endl;

        while (true)
        {
            if (getch() == '\n')
            {
                break;
            }
        }

        cout << "======================" << endl;
        cout << "遊戲開始！快攻擊怪物！" << endl;

        auto startTime = steady_clock::now();
        auto endTime = startTime + seconds(durationInSeconds);

        roundClicks = 0;

        while (steady_clock::now() < endTime)
        {
            if (getch() == '\n')
            {
                roundClicks++;
                totalClicks++;
                if (monsterHP > 0)
                {
                    monsterHP = max(0, monsterHP - 1);
                }

                currentVisibleLines = max(1, (monsterHP * static_cast<int>(monster.size())) / maxHP);

                clearScreen();
                printMonster();

                if (monsterHP == 0)
                {
                    cout << "怪物狀態：" << endl;
                    cout << "   已消失" << endl;
                    cout << "======================" << endl;
                    cout << "CONGRATULATION！" << endl;
                    cout << "恭喜你成功擊敗怪物！" << endl;

                    int reward = rand() % 100 + 1;
                    cout << "總計點擊次數：" << totalClicks << " 次！" << endl;
                    cout << "你獲得了 " << reward << " 金幣！" << endl;
                    return;
                }
            }
        }

        if (monsterHP > 0)
        {
            cout << "\n======================" << endl;
            cout << "FAILED！" << endl;
            cout << "你未能在時間內擊敗怪物！" << endl;
            cout << "總計點擊次數：" << totalClicks << " 次！" << endl;

            if (hasRevive)
            {
                cout << "你有一張免死金牌！請問你要使用嗎？（y/n）: ";
                char lastChar = getLastInput();

                if (lastChar == 'y' || lastChar == 'Y')
                {
                    hasRevive = false;
                    cout << "免死金牌已被使用。" << endl;
                    playGame(); // 再次调用自己
                }
                else
                {
                    cout << "======================" << endl;
                    cout << "GAME OVER！怪物未被擊敗，你沒有獲得任何金幣！" << endl;
                }
            }
            else
            {
                cout << "======================" << endl;
                cout << "GAME OVER！怪物未被擊敗，你沒有獲得任何金幣！" << endl;
            }
        }
    }
};

int main()
{
    click_game game;
    game.playGame();
    return 0;
}

/*
int main() {
    srand(static_cast<unsigned int>(time(0)));
    bool hasRevive = true;
    int monsterHP = 30;
    int totalClicks = 0;
    int roundClicks = 0;
    int currentVisibleLines;
    playGame(hasRevive, monsterHP, totalClicks, roundClicks, currentVisibleLines, monsterHP, true);
    return 0;
}
*/
