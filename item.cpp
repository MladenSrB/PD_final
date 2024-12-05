/* Version: 04.1517 */
#include "item.h"
#include "character.h"
using namespace std;

Item::Item(string aItemType, string aItemName, int price)
{
    itemName = aItemName;
    itemType = aItemType;
    this->price = price;
}

void Item::useItem() {}

void Item::print()
{
    cout << "Invoked print() of Item. Please invoke child class's print function.";
    exit(1);
}

//Potion::Potion(string itemType, string itemName, int price) : Item(itemType, itemName, price) { potionType = itemType; };

void AttackPotion::useItem(Character &aChar) { aChar.setatk(potionStrength + aChar.getatk()); }

void HealthPotion::useItem(Character &aChar) { aChar.sethp(potionStrength + aChar.gethp()); }

void AttackPotion::print() { cout << itemName << "| " << "Strength: " << potionStrength << " ; $ " << price; }

void HealthPotion::print() { cout << itemName << "| " << "Strength: " << potionStrength << " ; $ " << price; }

//Weapon::Weapon(int damage, int price) : Item("Weapon", "武器", price), baseATK(damage) {}

void Weapon::useWeapon(Character &aChar){ aChar.setatk(baseATK + aChar.getatk()); }

void Weapon::print() { cout << itemName << "| " << "ATK: " << baseATK << " | $ " << price; }

void Shield::print() { cout << itemName; }