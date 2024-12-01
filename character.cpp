#include "redistributable.h"
#include "item.h"
#include "character.h"

using namespace std;

Character::Character(string name)
{
    this->name = name;
    hp = 100;
    level = 1;
    atk = 5;
};

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