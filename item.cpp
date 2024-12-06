/* Version: 06.1520 */
#include "item.h"
#include "character.h"
using namespace std;

Item::Item(string aItemType, string aItemName, int price)
{
    itemName = aItemName;
    itemType = aItemType;
    this->price = price;
}

void Item::useItem(Player &aPlayer) {}

void Item::print()
{
    cout << "Invoked print() of Item. Please invoke child class's print function.";
    exit(1);
}

// Potion::Potion(string itemType, string itemName, int price) : Item(itemType, itemName, price) { potionType = itemType; };

void AttackPotion::useItem(Player &aPlayer) { aPlayer.setatk(potionStrength + aPlayer.getatk()); }

void HealthPotion::useItem(Player &aPlayer) { aPlayer.sethp(potionStrength + aPlayer.gethp()); }

void AttackPotion::print() { cout << itemName << "|| " << "Strength: " << potionStrength << " / $ " << price; }

void HealthPotion::print() { cout << itemName << "|| " << "Strength: " << potionStrength << " / $ " << price; }

// Weapon::Weapon(int damage, int price) : Item("Weapon", "武器", price), baseATK(damage) {}

void Weapon::useItem(Player &aPlayer) { aPlayer.setatk(baseATK + aPlayer.getatk()); }

void Weapon::print() { cout << itemName << "|| " << "ATK: " << baseATK << " / $ " << price; }

void Shield::useItem(Player &aPlayer)
{
    cout << "使用盾牌，保護時效內移動將不會損耗任何HP，效果將持續 " << validTime << " 秒！" << endl;
    aPlayer.setShieldTime(validTime);
}

void Shield::print() { cout << itemName << "|| " << "validTime: " << validTime << " / $ " << price; }