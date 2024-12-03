#include "clickgame.h"
#include "character.h"
#include "item.h"

using namespace std;
using namespace chrono;

clickgame::clickgame()
{
    srand(static_cast<unsigned int>(time(0))); // 初始化隨機數種子
    int randomVar = rand(); // 生成隨機變數
    int remainder = randomVar % 3; // 計算餘數
    this->totalClicks = 0;
    this->roundClicks = 0;
    this->currentVisibleLines = 0;
    if(remainder == 0)
    {
        string mname;
        Monster monster(mname, 1);
        this->monsterHP = monster.gethp();
        this->monsterATK = monster.getatk();
        this->monsterlv = monster.getLevel();
    }
    else if (remainder == 1)
    {
        string mname;
        Monster monster(mname, 2);
        this->monsterHP = monster.gethp();
        this->monsterATK = monster.getatk();
        this->monsterlv = monster.getLevel();
    }
    else
    {
        string mname;
        Monster monster(mname, 3);
        this->monsterHP = monster.gethp();
        this->monsterATK = monster.getatk();
        this->monsterlv = monster.getLevel();
    }
}


void clickgame::setNonBlockingInput(bool enable)
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

bool clickgame::isKeyPressed()
{
    struct timeval tv = {0L, 0L};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0;
}

char clickgame::getKey()
{
    char c;
    read(STDIN_FILENO, &c, 1);
    return c;
}


void clickgame::clearScreen()
{
    for (int i = 0; i < 50; ++i)
    {
        cout << endl;
    }
}

char clickgame::getLastInput()
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

void clickgame::printMonster(int visibleLines)
{
    cout << "\n怪物狀態：" << endl;
    for (int i = 0; i < visibleLines; i++)
    {
        cout << monster[i] << endl;
    }
    cout << endl;
}

void clickgame::playGame(Player &name)
{
    int maxHP = monsterHP;
    const int durationInSeconds = 10;
    srand(static_cast<unsigned int>(time(0)));
    
    // 根據等級設定怪物圖案
    if (monsterlv == 1)
    {
        monster = {
            "   (o_o)   ",
            "  <|   |>  ",
            "  /|   |\\  ",
        };
    }
    else if (monsterlv == 2)
    {
        monster = {
            "   (o_o)   ",
            "  <|   |>  ",
            "  /|   |\\  ",
            " /_|   |_\\ ",
        };
    }
    else
    {
        monster = {
            "   (o_o)   ",
            "  <|   |>  ",
            "   |   |   ",
            "  /|   |\\  ",
            " /_|   |_\\ ",
        };
    }

    currentVisibleLines = static_cast<int>(monster.size());

    cout << "===== 鍵盤連點遊戲 =====" << endl;
    cout << "一隻怪物出現了！" << endl;
    printMonster(currentVisibleLines);
    cout << "=== 規則 ==="<<endl;
    cout << "在限定時間內快速點擊Enter 鍵來攻擊怪物！" << endl;
    cout << "每點擊一次Enter 鍵即可減少怪物" << name.getatk() << "HP" << endl;
    cout << "這隻怪物總共有" << monsterHP << "HP！" << endl;
    cout << "你有 " << durationInSeconds << " 秒的時間擊敗它！" << endl;
    cout << "按下 Enter 鍵開始..." << endl;

    while (true)
    {
        if (isKeyPressed() && getKey() == '\n')
        {
            break;
        }
    }
    cout << "======================"<<endl;
    cout << "遊戲開始！快攻擊怪物！" << endl;

    auto startTime = steady_clock::now();
    auto endTime = startTime + seconds(durationInSeconds);
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
                    monsterHP = max(0, monsterHP - name.getatk());
                }

                currentVisibleLines = max(1, (monsterHP * static_cast<int>(monster.size())) / maxHP);

                clearScreen();
                printMonster(currentVisibleLines);

                if (monsterHP == 0)
                {
                    if(monsterlv == 1)
                    {
                        cout << "怪物狀態：" << endl;
                        cout << "   已消失" << endl;
                        cout << "======================"<<endl;
                        cout << "CONGRATULATION！" << endl;
                        cout << "恭喜你成功擊敗怪物！" << endl;
                        
                        cout << "總計點擊次數：" << totalClicks << " 次！" << endl;
                        name.addCoin(100);
                        name.addExp(100);
                        cout << "coin + 100" << "," << " " <<  "total coin :" << name.getCoin() <<  endl;
                        cout << "exp + 100" << "," << " " << "total exp :" << name.getExp() << endl;
                        cout << "hp - 0" << "," << " " << "total hp :" << name.gethp() << endl;
                        return;
                    }
                    else if (monsterlv == 2)
                    {
                        cout << "怪物狀態：" << endl;
                        cout << "   已消失" << endl;
                        cout << "======================"<<endl;
                        cout << "CONGRATULATION！" << endl;
                        cout << "恭喜你成功擊敗怪物！" << endl;
                        
                        cout << "總計點擊次數：" << totalClicks << " 次！" << endl;
                        name.addCoin(200);
                        name.addExp(200);
                        cout << "coin + 200" << "," << " " <<  "total coin :" << name.getCoin() <<  endl;
                        cout << "exp + 200" << "," << " " << "total exp :" << name.getExp() << endl;
                        cout << "hp - 0" << "," << " " << "total hp :" << name.gethp() << endl;
                        return;
                    }
                    else
                    {
                        cout << "怪物狀態：" << endl;
                        cout << "   已消失" << endl;
                        cout << "======================"<<endl;
                        cout << "CONGRATULATION！" << endl;
                        cout << "恭喜你成功擊敗怪物！" << endl;
                        
                        cout << "總計點擊次數：" << totalClicks << " 次！" << endl;
                        name.addCoin(300);
                        name.addExp(300);
                        cout << "coin + 300" << "," << " " <<  "total coin :" << name.getCoin() <<  endl;
                        cout << "exp + 300" << "," << " " << "total exp :" << name.getExp() << endl;
                        cout << "hp - 0" << "," << " " << "total hp :" << name.gethp() << endl;
                        return;
                    }
                }
            }
        }
        this_thread::sleep_for(milliseconds(10));
    }

    if (monsterHP > 0)
    {
        if(monsterlv == 1)
        {
            cout << "\n======================"<<endl;
            name.decreaseHp(monsterATK);
            name.addCoin(10);
            name.addExp(10);
            cout << "FAILED！" << endl;
            cout << "你未能在時間內擊敗怪物！" << endl;
            cout << "總計點擊次數：" << totalClicks << " 次！" << endl;
            cout << "coin + 10" << "," << " "  << "total coin :" << name.getCoin() << endl;
            cout << "exp + 10" << "," << " " << "total exp :" << name.getExp() << endl;
            cout << "hp" << " - " << monsterATK << "," << " " << "total hp :" << name.gethp() << endl;
            cout << "GAME OVER! 遊戲結束！" << endl;
            return;
        }
        else if (monsterlv == 2)
        {
            cout << "\n======================"<<endl;
            name.decreaseHp(monsterATK);
            name.addCoin(20);
            name.addExp(20);
            cout << "FAILED！" << endl;
            cout << "你未能在時間內擊敗怪物！" << endl;
            cout << "總計點擊次數：" << totalClicks << " 次！" << endl;
            cout << "coin + 20" << "," << " "  << "total coin :" << name.getCoin() << endl;
            cout << "exp + 20" << "," << " " << "total exp :" << name.getExp() << endl;
            cout << "hp" << " - " << monsterATK << "," << " " << "total hp :" << name.gethp() << endl;
            cout << "GAME OVER! 遊戲結束！" << endl;
            return;
        }
        else
        {
            cout << "\n======================"<<endl;
            name.decreaseHp(monsterATK);
            name.addCoin(30);
            name.addExp(30);
            cout << "FAILED！" << endl;
            cout << "你未能在時間內擊敗怪物！" << endl;
            cout << "總計點擊次數：" << totalClicks << " 次！" << endl;
            cout << "coin + 30" << "," << " "  << "total coin :" << name.getCoin() << endl;
            cout << "exp + 30" << "," << " " << "total exp :" << name.getExp() << endl;
            cout << "hp" << " - " << monsterATK << "," << " " << "total hp :" << name.gethp() << endl;
            cout << "GAME OVER! 遊戲結束！" << endl;
            return;
        }
    }
}
/*int main()
{
    clickgame game;
    // 初始化玩家名稱
    string playerName;
    Player player(playerName); // 創建玩家物件

    // 開始遊戲
    game.playGame(player);
    return 0;
}*/

