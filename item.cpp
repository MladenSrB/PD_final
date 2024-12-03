/* Version: 03.2001 */
#include "item.h"
using namespace std;


Item::Item(string aItemType, string aItemName)
{
    itemName = aItemName;
    itemType = aItemType;
}

Potion::Potion(string itemType, string itemName) : Item(itemType, itemName) { potionType = itemType; };
void Weapon::useWeapon(Character &aChar) {
    aChar.setatk(baseATK + aChar.getatk());
}