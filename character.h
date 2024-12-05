/* Version: 04.1517 */

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

    chrono::time_point<chrono::steady_clock> shieldStartTime;
    bool isShieldActive;
    int shieldDuration;

public:
    Character(string name);
    void attack(Character c, int atkpt);
    void setatk(int baseATK) { atk = baseATK; };
    int getatk() { return atk; };
    void sethp(int newhp) { hp = newhp; };
    int gethp() { return hp; };
    int getlevel() { return level; };

    void setShieldTime(int durationInSeconds);
    int getShieldRemainingTime();
    bool checkShieldStatus();
    void move();
};

class Player : public Character
{
private:
    Item **backpack;
    int backpackLimit;
    int backpackCount;
    int exp;
    int x;
    int y;
    int coin;
    static const int EXP_LV = 100;

public:
    Player(string name);
    ~Player();
    void upgrade();
    
    bool boughtItem(Item *item);
    bool usedItem(int usedIndex);
    void printBackpack();
    int getBackpackLimit() { return backpackLimit; };
    int getBackpackCount() { return backpackCount; };
    
    void addCoin(int c) { coin += c; };
    int getCoin() { return coin; };
    void decreaseCoin(int c) { coin -= c; };
 
    void addExp(int pt);
    int getExp() { return exp; };
    void decreaseHp(int blood) { hp -= blood; };
    void addHp(int blood) { hp += blood; };

    int getShieldRemainingTime() { return Character::getShieldRemainingTime(); }
    
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
    void printAllGoods();
};

#endif