/* Version: 06.1746 */

/* CHANGE LOG */

// ver. 03.1846 : cleaned up included headers and libraries
// ver. 03.2001 : added Merchant class

#include "character.h"
#include "item.h"
#include "backpack.h"

using namespace std;

Character::Character(string name)
{
    this->name = name;
    hp = 100;
    level = 1;
    atk = 5;
    isShieldActive = false;
    shieldDuration = 0;
}

void Character::attack(Character c, int atkpt)
{
    c.hp -= atkpt;
}

void Character::setShieldTime(int durationInSeconds)
{
    shieldStartTime = chrono::steady_clock::now();
    isShieldActive = true;
    shieldDuration = durationInSeconds;
}

int Character::getShieldRemainingTime()
{
    if (isShieldActive)
    {
        auto currentTime = chrono::steady_clock::now();
        // 計算已經過的時間
        auto elapsedTime = chrono::duration_cast<chrono::seconds>(currentTime - shieldStartTime).count();
        // 類型一致
        return max(0, shieldDuration - static_cast<int>(elapsedTime));
    }
    return 0;
}

// 每次玩家移動時，檢查是否盾牌還有效
bool Character::checkShieldStatus()
{
    if (isShieldActive)
    {
        auto currentTime = chrono::steady_clock::now();
        auto elapsedTime = chrono::duration_cast<chrono::seconds>(currentTime - shieldStartTime).count();

        if (elapsedTime >= shieldDuration)
        {
            isShieldActive = false;
            return false;
        }
        return true;
    }
    return false;
}

void Character::move()
{
    if (!checkShieldStatus())
    {
        hp--;
    }
}

Player::Player(string name) : Character(name)
{
    coin = 0;
    exp = 0;
    int backpackLimit = 3;
    backpack = new Backpack(3);
}

Player::~Player()
{
    backpack->~Backpack();
}

void Player::upgrade()
{
    level++;
    atk += 5;
}

bool Player::boughtItem(Item *item)
{
    if (backpack->getBackpackCount() >= backpack->getBackpackLimit())
    {
        cout << "\n𖠋 Merchant：你的包包滿了！\n";
        std::this_thread::sleep_for(std::chrono::seconds(2));
        return false;
    }

    int itemPrice = item->getPrice();
    if (getCoin() < itemPrice)
    {
        cout << "\n𖠋 Merchant：喂！小子，你的錢不夠啊！\n";
        std::this_thread::sleep_for(std::chrono::seconds(2));
        return false;
    }
    else
    {
        decreaseCoin(itemPrice);
        cout << "\n𖠋 Merchant：真識貨， " << item->getName() << " 就交給你啦！\n";
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    backpack->addItem(*item);
    return true;
}

bool Player::usedItem(int usedIndex)
{
    if (usedIndex < 0 || usedIndex >= backpack->getBackpackCount())
    {
        cout << "輸出無效\n";
        return false;
    }

    // 使用物品
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cout << "使用中..."; // 待：新增使用的商品說明
    /*backpack[usedIndex]->print();
    cout << endl;*/

    backpack->useItem(usedIndex, *this);

    return true;
}

void Player::openBackpack()
{
    backpack->openBackpack(*this);
}

void Player::addExp(int pt)
{
    exp += pt;
    while (this->exp >= pow(this->level, 2) * EXP_LV)
        upgrade();
}

Monster::Monster(string name, int level) : Character(name)
{
    this->level = level;
    this->hp *= pow(1.25, (level - 1));
    this->atk += 5 * (level - 1);
}

// Merchant's constructor
Merchant::Merchant() : Character("Merchant")
{
    // set up basic info in class Character
    hp = 100;
    level = 99;
    atk = 0;

    // generate Merchant's goods
    // 目前會產生4個商品，每種商品各1個
    goods = new Item *[goodsCnt];

    srand(time(NULL));
    goods[0] = new HealthPotion(rand() % (25) + 5, 15); // strength of the poisons are randomly assigned within 5~15
    goods[1] = new LuckyPotion(rand() % (5) + 10, 20);
    goods[2] = new Weapon(rand() % (10) + 1, 10); // 產生ATK 1~10的武器
    goods[3] = new Shield(rand() % (10) + 1, 20);
}

Merchant::~Merchant()
{
    for (int i = 0; i < goodsCnt; ++i)
    {
        delete goods[i];
    }
    delete[] goods;
}

Item *Merchant::sellGood(int itemIndex)
{
    Item *selectedItem = goods[itemIndex];
    goods[itemIndex] = nullptr;
    return selectedItem;
}

void Merchant::printAllGoods()
{
    for (int i = 0; i < goodsCnt; i++)
    {
        cout << "[" << i << "] ";
        goods[i]->print();
        cout << '\n';
    }
}