/* Version: 06.1700 */

/* CHANGE LOG */

// ver. 06.1520 : added backpack system

#ifndef BACKPACK_H
#define BACKPACK_H
#include "character.h"
#include "item.h"
class Backpack
{
private:
    Item **items;
    int backpackCount;
    int backpackLimit;

public:
    Backpack();
    Backpack(int limit);
    ~Backpack();
    int getBackpackLimit() { return backpackLimit; };
    int getBackpackCount() { return backpackCount; };
    void addItem(Item &item);
    void expandSize(int newBlockCnt);
    void useItem(int itemIndex, Player &aPlayer);
    void openBackpack(Player &player);
    void render(Item **items, int cursor, int screenWidth);

    char getch();
    void printCentered(const std::string &text, int width);
};

#endif