#include "redistributable.h"

using namespace std;
using namespace chrono;

class click_game()
{
protected:
    bool hasRevive = true;
    int monsterHP = 30;
    int totalClicks = 0;
    int roundClicks = 0;
    int currentVisibleLines;

public:
    bool isKeyPressed();
    char getKey();
    void setNonBlockingInput(bool enable);
    bool isKeyPressed();
    char getKey();
    void clearScreen();
    char getLastInput();
    void printMonster(const vector<string> &monster, int visibleLines);
    void playGame(bool &hasRevive, int &monsterHP, int &totalClicks, int &roundClicks, int &currentVisibleLines, int maxHP, bool isFirstRun);
}

// constructor
click_game::click_game(bool hasRevive, int monsterHP, int totalClicks, int roundClicks)
{
    this->hasRevive = true;
    this->monsterHP = 30;
    this->totalClicks = 0;
    this->roundClicks = 0;
}

// 跨平台按鍵檢測函數
#ifdef _WIN32
click_game::bool isKeyPressed()
{
    return _kbhit();
}
click_game::char getKey()
{
    return _getch();
}
#else
click_game::void setNonBlockingInput(bool enable)
{
    static struct termios oldt, newt;
    if (enable)
    {
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    }
    else
    {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }
}

click_game::bool isKeyPressed()
{
    struct timeval tv = {0L, 0L};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0;
}

click_game::char getKey()
{
    char c;
    read(STDIN_FILENO, &c, 1);
    return c;
}
#endif

click_game::void clearScreen()
{
    for (int i = 0; i < 50; ++i)
    {
        cout << endl;
    }
}

click_game::char getLastInput()
{
    string input;
    char lastValidChar = '\0';
    getline(cin, input);
    for (char c : input)
    {
        if (!isspace(c))
        { // 檢查非空白字元
            lastValidChar = c;
        }
    }
    return tolower(lastValidChar); // 將最後有效字元轉為小寫
}

click_game::void printMonster(const vector<string> &monster, int visibleLines)
{
    cout << "\n怪物狀態：" << endl;
    for (int i = 0; i < visibleLines; i++)
    {
        cout << monster[i] << endl;
    }
    cout << endl;
}

click_game::void playGame(bool &hasRevive, int &monsterHP, int &totalClicks, int &roundClicks, int &currentVisibleLines, int maxHP, bool isFirstRun)
{
    const int durationInSeconds = 10;

    vector<string> monster =
        {
            "   (o_o)   ",
            "  <|   |>  ",
            "   |   |   ",
            "  /|   |\\  ",
            " /_|   |_\\ ",
        };

    if (isFirstRun)
    {
        currentVisibleLines = static_cast<int>(monster.size());

        cout << "===== 鍵盤連點遊戲 =====" << endl;
        cout << "一隻怪物出現了！" << endl;
        printMonster(monster, currentVisibleLines);
        cout << "=== 規則 ===" << endl;
        cout << "在限定時間內快速點擊Enter 鍵來攻擊怪物！" << endl;
        cout << "每點擊一次Enter 鍵即可減少怪物 1 HP" << endl;
        cout << "這隻怪物總共有 30 HP！" << endl;
        cout << "你有 " << durationInSeconds << " 秒的時間擊敗它！" << endl;
        cout << "按下 Enter 鍵開始..." << endl;

        while (true)
        {
            if (isKeyPressed() && getKey() == '\n')
            {
                break;
            }
        }
        cout << "======================" << endl;
        cout << "遊戲開始！快攻擊怪物！" << endl;
    }
    else
    {
        cout << "\n======================" << endl;
        cout << "繼續挑戰怪物！" << endl;
        printMonster(monster, currentVisibleLines);
    }

    auto startTime = steady_clock::now();
    auto endTime = startTime + seconds(durationInSeconds);

#ifdef __unix__
    setNonBlockingInput(true);
#endif

    roundClicks = 0;

    while (steady_clock::now() < endTime)
    {
        if (isKeyPressed())
        {
            char key = getKey();
            if (key == '\n')
            {
                roundClicks++;
                totalClicks++;
                if (monsterHP > 0)
                {
                    monsterHP = max(0, monsterHP - 1);
                }

                currentVisibleLines = max(1, (monsterHP * static_cast<int>(monster.size())) / maxHP);

                clearScreen();
                printMonster(monster, currentVisibleLines);

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
#ifdef __unix__
                    setNonBlockingInput(false);
#endif
                    return;
                }
            }
        }
        this_thread::sleep_for(milliseconds(10));
    }

#ifdef __unix__
    setNonBlockingInput(false);
#endif

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
                playGame(hasRevive, monsterHP, totalClicks, roundClicks, currentVisibleLines, maxHP, false);
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

int main()
{
    srand(static_cast<unsigned int>(time(0)));
    bool hasRevive = true;
    int monsterHP = 30;
    int totalClicks = 0;
    int roundClicks = 0;
    int currentVisibleLines;
    playGame(hasRevive, monsterHP, totalClicks, roundClicks, currentVisibleLines, monsterHP, true);
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
