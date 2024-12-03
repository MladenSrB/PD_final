/* Version: 03.2001 */

/* CHANGE LOG */

// ver. 03.1846 : cleaned up included headers and libraries
// ver. 03.2001 : added Merchant class

#include "character.h"
#include "item.h"

using namespace std;

Character::Character(string name)
{
    this->name = name;
    hp = 100;
    level = 1;
    atk = 5;
}

void Character::attack(Character c, int atkpt)
{
    c.hp -= atkpt;
}

Player::Player(string name) : Character(name)
{
    coin = 0;
    exp = 0;
    backpack_limit = 3;
    backpack = new Item *[backpack_limit];
}

void Player::upgrade()
{
    level++;
    atk += 5;
    backpack_limit++;
    Item **copyBackpack = new Item *[backpack_limit];
    for (int i = 0; i < backpack_limit - 1; ++i)
    {
        copyBackpack[i] = backpack[i];
    }
    for (int i = 0; i < backpack_limit - 1; ++i)
    {
        delete backpack[i];
    }
    delete[] backpack;
    backpack = copyBackpack;
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
    goods[0] = new HealthPotion(rand() % (15 - 5 + 1) + 5); // strength of the poisons are randomly assigned within 5~15
    goods[1] = new AttackPotion(rand() % (15 - 5 + 1) + 5);
    goods[2] = new Weapon("Weapon", rand() % (10 - 1 + 1) + 1); // 產生ATK 1~10的武器
    goods[3] = new Shield("Shield");
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
    Item *temp = goods[itemIndex];
    goods[itemIndex] = nullptr;

    return temp;
}

void Merchant::printAllGoods(const int itemIndex)
{
    for (int i = 0; i < goodsCnt; i++)
    {
        cout << "[" << itemIndex << "] ";
        goods[i]->print();
        cout << '\n';
    }
}