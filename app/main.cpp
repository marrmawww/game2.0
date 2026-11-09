#include <iostream>
#include <string>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#endif

#include "GameController.h"
#include "NeutralOperators.h"
#include "Logger.h"

using namespace std;

void printBase(const Base& base) {
    cout << base.getName() << ": здоровье " << base.getHealth()
         << ", юнитов " << base.getAliveCount() << "/" << base.getMaxUnits() << endl;
}

void printCheck(const string& name, bool passed) {
    cout << (passed ? "[OK]   " : "[FAIL] ") << name << endl;
}

vector<string> createMap() {
    vector<string> map;
    map.push_back("....*****.....^^....");
    map.push_back("....*****....^^^....");
    map.push_back("..........~~..^^....");
    map.push_back("..~~......~~~.......");
    map.push_back(".~~~...***..........");
    map.push_back("......****.....^....");
    map.push_back("......***.....^^^...");
    map.push_back("..........~~~~......");
    map.push_back("...^^.....~~~~......");
    map.push_back("...^^^........***...");
    map.push_back("...........~.....***");
    map.push_back("....................");
    return map;
}

int main() {
#ifdef _WIN32
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
#endif

    openLog("game.log");

    GameController game(20, 12);
    game.map().loadFromText(createMap());

    game.map().placeNeutralObject(5, 3, unique_ptr<NeutralObject>(new TreasureChest()));
    game.map().placeNeutralObject(6, 3, unique_ptr<NeutralObject>(new Trap(20)));
    game.map().placeNeutralObject(7, 3, unique_ptr<NeutralObject>(new Altar()));
    game.map().placeNeutralObject(8, 3, unique_ptr<NeutralObject>(new EnergySource()));

    Base& blue = game.addBase("Синяя база", 'B', 1, 2, 100, 10, 2);
    Base& red = game.addBase("Красная база", 'R', 17, 9, 100, 10, 2);

    cout << "\n1. Создание юнитов через фабрику\n";
    shared_ptr<Unit> swordsman = game.spawnUnit(blue, UnitKind::Swordsman, 2, 2);
    shared_ptr<Unit> archer = game.spawnUnit(blue, UnitKind::LightArcher, 3, 2);
    shared_ptr<Unit> cavalry = game.spawnUnit(blue, UnitKind::Cataphract, 9, 2);
    game.spawnUnit(red, UnitKind::HeavyArcher, 16, 9);
    printBase(blue);
    game.printMap(cout);

    cout << "\n2. Перемещение по ландшафту и Proxy\n";
    game.moveUnit(*archer, 1, 0);
    game.moveUnit(*cavalry, 0, 1);
    const TerrainProxy& proxy = game.map().getProxy(13, 1);
    proxy.isPassable(UnitKind::Swordsman);
    proxy.isPassable(UnitKind::Swordsman);
    cout << "Обращений к ландшафту: " << proxy.cacheMisses()
         << ", попаданий в кеш: " << proxy.cacheHits() << endl;

    cout << "\n3. Нейтральные объекты и Strategy\n";
    shared_ptr<Unit> chestUnit = game.spawnUnit(blue, UnitKind::Spearman, 5, 2);
    shared_ptr<Unit> trapUnit = game.spawnUnit(blue, UnitKind::HorseArcher, 6, 2);
    if (chestUnit) {
        game.moveUnit(*chestUnit, 0, 1);
        game.interactWithObject(*chestUnit); // unit + сундук
    }
    if (trapUnit) {
        game.moveUnit(*trapUnit, 0, 1);
        game.interactWithObject(*trapUnit); // unit - ловушка
    }

    Altar altar;
    *swordsman * altar;
    *archer * altar;
    *cavalry * altar;

    EnergySource source;
    if (trapUnit) *trapUnit / source;

    cout << "\n4. Бой, уничтожение юнита и итоговые проверки\n";
    shared_ptr<Unit> enemy = game.spawnUnit(red, UnitKind::HeavyArcher, 10, 8);
    shared_ptr<Unit> target = game.spawnUnit(blue, UnitKind::LightArcher, 10, 9);
    if (enemy && target) {
        while (target->isAlive()) game.attackUnit(*enemy, *target);
    }

    const Terrain& mountains = game.map().getRealTerrain(14, 0);
    const Terrain& forest = game.map().getRealTerrain(4, 0);
    const Terrain& swamp = game.map().getRealTerrain(10, 2);
    printCheck("Горы недоступны мечнику", !mountains.isPassable(UnitKind::Swordsman));
    printCheck("Лес доступен мечнику", forest.isPassable(UnitKind::Swordsman));
    printCheck("Болото замедляет мечника", swamp.getSpeedModifier(UnitKind::Swordsman) < 1.0);
    printCheck("Счётчик синей базы равен 5", blue.getAliveCount() == 5);
    printCheck("История гибели содержит 1 запись", blue.getDestroyedHistory().size() == 1);

    printBase(blue);
    cout << "Событий об уничтожении юнитов: " << blue.getDestroyedHistory().size() << endl;
    game.printMap(cout);
    game.endTurn();

    closeLog();
    cout << "\nРабота программы завершена. События записаны в game.log.\n";
    return 0;
}
