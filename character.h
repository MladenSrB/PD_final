/* Version: 03.2001 */

/* CHANGE LOG */

// ver. 03.1846 : cleaned up included headers and libraries
// ver. 03.2001 : added Merchant class

#ifndef CHARACTER_H
#define CHARACTER_H

#include "redistributable.h"
#include "item.h"

using namespace std;

class Item; // 前向宣告

class Character
{
protected:
    string name; // 名字
    int hp;      // 血條
    int level;   // 等級
    int atk;     // 攻擊力

public:
    Character(string name);
    void attack(Character c, int atkpt);
    void setatk(int baseATK) { atk = baseATK; };
    int getatk() { return atk; };
    void sethp(int newhp) { hp = newhp; };
    int gethp() { return hp; };
};

class Player : public Character
{
private:
    Item **backpack;
    int backpack_limit;
    int exp;
    int x;
    int y;
    int coin;
    static const int EXP_LV = 100;

public:
    Player(string name);
    void upgrade();
    void addItem(Item i);
    bool usedItem(Item i);
    void addCoin(int c) { coin += c; };
    int getCoin() { return coin; };

    void addExp(int pt);
    void decreaseHp(int blood) { hp -= blood; };
    void addHp(int blood) { hp += blood; };
    int getExp() { return exp; };
};

class Monster : public Character
{
public:
    Monster(string name, int level);
    int getLevel() { return level; };
};

class Merchant : public Character
{
protected:
    int goodsCnt = 4; // 商店的商品數量
    Item **goods;     // 商店中陳列的物品陣列

public:
    Merchant(); // constructor
    ~Merchant();
    // Item addGood();
    Item *sellGood(int itemIndex);
    void printAllGoods(const int itemIndex);
};

#endif