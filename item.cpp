/* Version: 04.1517 */
#include "item.h"
#include "character.h"
using namespace std;

Item::Item(std::string aItemType, std::string aItemName)
{
    itemName = aItemName;
    itemType = aItemType;
}

void Item::useItem() {}

void Item::print()
{
    cout << "Invoked print() of Item. Please invoke child class's print function.";
    exit(1);
}

Potion::Potion(std::string itemType, std::string itemName) : Item(itemType, itemName) { potionType = itemType; };

void AttackPotion::useItem(Character &aChar) { aChar.setatk(potionStrength + aChar.getatk()); }

void HealthPotion::useItem(Character &aChar) { aChar.sethp(potionStrength + aChar.gethp()); }

void AttackPotion::print() { cout << itemName << "Strength: " << potionStrength; }

void HealthPotion::print() { cout << itemName << "Strength: " << potionStrength; }

Weapon::Weapon(std::string weaponName, int damage) : Item("Weapon", weaponName), baseATK(damage) {}

void Weapon::useWeapon(Character &aChar){ aChar.setatk(baseATK + aChar.getatk()); }

void Weapon::print() { cout << itemName << "ATK: " << baseATK; }

void Shield::print() { cout << itemName; }