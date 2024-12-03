#ifndef ITEM_H
#define ITEM_H

#include "redistributable.h"
#include "character.h"
using namespace std;

class Character;

class Item
{
protected:
    string itemType;
    string itemName;

public:
    virtual void useItem() {};
    Item(string aItemType, string aItemName);
};

class Potion : public Item
{
protected:
    string potionType;

public:
    Potion(string itemType, string itemName) ;
};

class Weapon : Item
{
protected:
    const int baseATK = -1;

public:
    Weapon(string weaponName, int damage) : Item("Weapon", weaponName), baseATK(damage) {};
    void useWeapon(Character &aChar);
};

class AttackPotion : public Potion
{
private:
    int potionStrength = -1;

public:
    AttackPotion(int strength) : Potion("Attack Potion", "攻擊藥水"), potionStrength(strength) {};
    ~AttackPotion() {};
    void useItem(Character &aChar) { aChar.setatk(potionStrength + aChar.getatk()); }
};

class HealthPotion : public Potion
{
private:
    int potionStrength = -1;

public:
    HealthPotion(int strength) : Potion("Health Potion", "生命藥水"), potionStrength(strength) {};
    ~HealthPotion() {};
    void useItem(Character &aChar) { aChar.sethp(potionStrength + aChar.gethp()); }
};

class Shield : public Item
{
public:
    Shield(string shieldName) : Item("Shield", shieldName) {};
};

#endif