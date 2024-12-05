/* Version: 04.1517 */

/* CHANGE LOG */

// ver. 03.1846 : cleaned up included headers and libraries
// ver. 03.2001 : added print()

#ifndef ITEM_H
#define ITEM_H

#include "redistributable.h"

using namespace std;

class Character;

class Item
{
protected:
    string itemType;
    string itemName;
    int price;

public:
    Item(string aItemType, string aItemName, int price);
    virtual void useItem(Character &aChar);
    virtual void print();
    int getPrice() { return price; }
};

class Potion : public Item
{
protected:
    string potionType;

public:
    Potion(string itemType, string itemName, int price) : Item(itemType, itemName, price) { potionType = itemType; };
    virtual void print()
    {
        cout << "Invoked print() of Item. Please invoke child class's print function.";
        exit(1);
    };
};

class Weapon : public Item //永久增加
{
protected:
    const int baseATK = -1;

public:
    Weapon(int damage, int price) : Item("Weapon", "武器", price), baseATK(damage) {};
    void useItem(Character &aChar);
    void print();
};

class AttackPotion : public Potion //待：僅增加存在一些時間
{
private:
    int potionStrength = -1;

public:
    AttackPotion(int strength, int price) : Potion("Attack Potion", "攻擊藥水", price), potionStrength(strength) {};
    ~AttackPotion() {};
    void useItem(Character &aChar);
    void print();
};

class HealthPotion : public Potion
{
private:
    int potionStrength = -1;

public:
    HealthPotion(int strength, int price) : Potion("Health Potion", "生命藥水", price), potionStrength(strength) {};
    ~HealthPotion() {};
    void useItem(Character &aChar);
    void print();
};
// 待：修改為時間內不夠hp
class Shield : public Item
{
private:
    int validTime;
public:
    Shield(int time, int price) : Item("Shield", "盾牌", price), validTime(time) {};
    void useItem(Character &aChar);
    void print();
    int getValidTime() { return validTime; } 
    void setValidTime(int time) { validTime = time; }
};


#endif