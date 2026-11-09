#pragma once
#include <memory>
#include <ostream>
#include <vector>
#include "Base.h"
#include "TerrainMap.h"
#include "Unit.h"

// Управляет картой, базами и юнитами. Все игровые действия идут через него.
class GameController {
public:
    GameController(int width, int height, int seed = 1);

    TerrainMap& map() { return map_; }
    const TerrainMap& map() const { return map_; }

    Base& addBase(const std::string& name, char symbol, int x, int y,
                  int health, size_t maxUnits, int defenseLevel);

    // создать юнита на клетке (проверяет границы, проходимость и занятость)
    std::shared_ptr<Unit> spawnUnit(Base& base, UnitType type, int x, int y);

    // Один шаг на соседнюю клетку (dx и dy от -1 до 1).
    // Проходимость и скорость проверяет Proxy.
    bool moveUnit(Unit& unit, int dx, int dy);

    bool attackUnit(Unit& attacker, Unit& target);
    bool attackBase(Unit& attacker, Base& base);

    // взаимодействие с объектом, на клетке которого стоит юнит
    bool interactWithObject(Unit& unit);

    void endTurn();
    int getTurn() const { return turn_; }

    // Обычная печать всего поля.
    void printMap(std::ostream& os) const;


private:
    char getCellSymbol(int x, int y) const;
    bool isCellFree(int x, int y) const;
    int nextRandom();   // псевдослучайное число от 0 до 99

    TerrainMap map_;
    std::vector<std::unique_ptr<Base> > bases_;
    long long seed_;
    int turn_;
};
