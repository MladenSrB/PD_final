#include "item.h"

Item::Item(string aItemType, string aItemName)
{
    itemName = aItemName;
    itemType = aItemType;
}

Potion(string itemType, string itemName) : Item(itemType, itemName) { potionType = itemType; };
void useWeapon(Character &aChar) { aChar.setatk(baseATK); }