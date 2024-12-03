/* Version: 03.2001 */

/* CHANGE LOG */

// ver. 03.1846 : cleaned up included headers and libraries
// ver. 03.2001 : added print()

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
    virtual void print()
    {
        cout << "Invoked print() of Item. Please invoke child class's print function.";
        exit(1);
    };
};

class Potion : public Item
{
protected:
    string potionType;

public:
    Potion(string itemType, string itemName);
    virtual void print()
    {
        cout << "Invoked print() of Item. Please invoke child class's print function.";
        exit(1);
    };
};

class Weapon : public Item
{
protected:
    const int baseATK = -1;

public:
    Weapon(string weaponName, int damage) : Item("Weapon", weaponName), baseATK(damage) {};
    void useWeapon(Character &aChar);
    void print() { cout << itemName << "ATK: " << baseATK; }
};

class AttackPotion : public Potion
{
private:
    int potionStrength = -1;

public:
    AttackPotion(int strength) : Potion("Attack Potion", "攻擊藥水"), potionStrength(strength) {};
    ~AttackPotion() {};
    void useItem(Character &aChar) { aChar.setatk(potionStrength + aChar.getatk()); }
    void print() { cout << itemName << "Strength: " << potionStrength; }
};

class HealthPotion : public Potion
{
private:
    int potionStrength = -1;

public:
    HealthPotion(int strength) : Potion("Health Potion", "生命藥水"), potionStrength(strength) {};
    ~HealthPotion() {};
    void useItem(Character &aChar) { aChar.sethp(potionStrength + aChar.gethp()); }
    void print() { cout << itemName << "Strength: " << potionStrength; }
};

class Shield : public Item
{
public:
    Shield(string shieldName) : Item("Shield", shieldName) {};
    void print() { cout << itemName; }
};

#endif