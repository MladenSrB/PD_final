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
    if (type == 0) { 
        int recoveredHp = potionStrength * (rand() % 2);
        aPlayer.sethp(recoveredHp + aPlayer.gethp());
        cout << "恭喜抽中生命藥水！恢復了 " << recoveredHp << " 點生命值！\n";
    }
    else if (type == 1) { 
        int increasedAtk = potionStrength * ((rand() % 2) / 5) * (rand() % 2);
        aPlayer.setatk(increasedAtk + aPlayer.getatk());
        cout << "恭喜抽中武器！攻擊力增加了 " << increasedAtk << " 點！\n";
    }
    else if (type == 2) { 
        int gainedExp = potionStrength * (rand() % 5) * (rand() % 2);
        aPlayer.setExp(gainedExp + aPlayer.getExp());
        cout << "恭喜抽中經驗值禮包！獲得了 " << gainedExp << " 點經驗值！\n";
    }
    else if (type == 3) { 
        int gainedCoin = potionStrength * (rand() % 5) * (rand() % 2);
        aPlayer.addCoin(gainedCoin);
        cout << "恭喜刮出 " << gainedCoin << " 金幣！\n";
    }
}


void HealthPotion::useItem(Player &aPlayer) { aPlayer.sethp(potionStrength + aPlayer.gethp()); }

void LuckyPotion::print() { cout << itemName <<  " : $ " << price; }

void HealthPotion::print() { cout << itemName << "|| " << "Strength: " << potionStrength << " / $ " << price; }

void Weapon::useItem(Player &aPlayer) { aPlayer.setatk(baseATK + aPlayer.getatk()); }

void Weapon::print() { cout << itemName << "|| " << "ATK: " << baseATK << " / $ " << price; }

void Shield::useItem(Player &aPlayer)
{
    cout << "使用盾牌，保護時效內移動將不會損耗任何HP，效果將持續 " << validTime << " 秒！" << endl;
    aPlayer.setShieldTime(validTime);
}

void Shield::print() { cout << itemName << "|| " << "validTime: " << validTime << " / $ " << price; }