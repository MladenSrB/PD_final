/* Version: 06.1700 */
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

void LuckyPotion::useItem(Player &aPlayer) 
{
    int type = rand() % 4;
    if (type == 0) { aPlayer.sethp(potionStrength + aPlayer.gethp()); }
    else if (type == 1) { aPlayer.setatk(potionStrength + aPlayer.getatk()); }
    else if (type == 2) { aPlayer.setExp(potionStrength + aPlayer.getExp()); }
    else if (type == 3) { aPlayer.addCoin(potionStrength); }
}

void HealthPotion::useItem(Player &aPlayer) { aPlayer.sethp(potionStrength + aPlayer.gethp()); }

void LuckyPotion::print() { cout << itemName << "|| " << "Strength: " << potionStrength << " / $ " << price; }

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