/* Version: 06.1746 */

/* CHANGE LOG */

// ver. 06.1520 : added backpack system

#include "backpack.h"
using namespace std;

char Backpack::getch()
{
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ~ICANON");
    return buf;
}

void Backpack::printCentered(const std::string &text, int width)
{
    int padding = (width - text.size()) / 2;
    if (padding > 0)
        std::cout << std::string(padding, ' ');
    std::cout << text;
}

// render backpack interface
void Backpack::render(Item **items, int cursor, int screenWidth)
{
    system("clear");
    int titleWidth = screenWidth - 4;

    printCentered("======= 背包系統 =======", screenWidth);
    std::cout << "\n\n";

    for (size_t i = 0; i < backpackCount; ++i)
    {
        std::cout << (i == cursor ? " > " : "   "); // cursor appearance
        items[i]->print();
        std::cout << "\n";
    }

    std::cout << "\n";
    printCentered("使用方向鍵選擇，Enter 確認使用", screenWidth);
    cout << '\n';
    printCentered("按【 esc 】回到遊戲", screenWidth);
    std::cout << "\n";
}

void Backpack::expandSize(int newBlockCnt)
{
    backpackLimit += newBlockCnt;
    Item **newBackpack = new Item *[backpackLimit];
    for (int i = 0; i < backpackCount; i++)
        delete items[i];
    delete[] items;
    items = newBackpack;
}

Backpack::Backpack()
{
    items = nullptr;
    backpackCount = 0;
    backpackLimit = 0;
}

Backpack::Backpack(int limit)
{
    backpackCount = 0;
    backpackLimit = 3;
    expandSize(3);
}

Backpack::~Backpack()
{
    delete[] items;
}

void Backpack::addItem(Item &item)
{
    items[backpackCount++] = &item;
}

void Backpack::useItem(int itemIndex, Player &aPlayer)
{
    items[itemIndex]->useItem(aPlayer);
    // 移除物品
    for (int i = itemIndex; i < backpackCount - 1; ++i)
    {
        items[i] = items[i + 1];
    }
    backpackCount--;
}

// 主程序
void Backpack::openBackpack(Player &player)
{
    int cursor = 0;             // cursor init pos
    const int screenWidth = 50; // terminal width

    while (true)
    {
        // 渲染界面
        render(items, cursor, screenWidth);

        // 捕获用户输入
        char input = getch();
        if (input == '\033')
        {                              
            char secondChar = getch(); // skip [
            if (secondChar == '[')
            {
                char thirdChar = getch();
                switch (thirdChar)
                {
                case 'A': // /033[A
                    cursor = (cursor - 1 + backpackCount) % backpackCount;
                case 'B': // /033[B
                    cursor = (cursor + 1) % backpackCount;
                }
            }
            else
            {
                break;
            }
        }
        else if (input == '\n')
        { // Enter
            if (backpackCount == 0)
            {
                break;
            }
            system("clear");
            std::cout << "你使用了 ";
            useItem(cursor, player);
            items[cursor]->print();
            std::cout << "！" << std::endl;
            cout << "按【 Enter 】回到遊戲" << endl;
            while (getch() != '\n')
            {
            }
            return;
        }
    }
}
