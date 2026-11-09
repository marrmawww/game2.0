#include "GameController.h"
#include <cstdlib>
#include <iostream>
#include "Logger.h"
#include "NeutralOperators.h"

GameController::GameController(int width, int height, int seed)
    : map_(width, height) {
    seed_ = seed;
    turn_ = 1;
}

// Генератор случайных чисел.
int GameController::nextRandom() {
    seed_ = (seed_ * 1103515245 + 12345) % 2147483648LL;
    return static_cast<int>((seed_ / 65536) % 100);
}

static int distanceBetween(int x1, int y1, int x2, int y2) {
    int dx = std::abs(x1 - x2);
    int dy = std::abs(y1 - y2);
    if (dx > dy) return dx;
    return dy;
}

Base& GameController::addBase(const std::string& name, char symbol, int x, int y,
                              int health, size_t maxUnits, int defenseLevel) {
    bases_.push_back(std::unique_ptr<Base>(new Base(name, symbol, x, y, health, maxUnits, defenseLevel)));
    return *bases_.back();
}

// Клетка свободна, если на ней нет базы и живого юнита.
bool GameController::isCellFree(int x, int y) const {
    for (size_t b = 0; b < bases_.size(); ++b) {
        if (bases_[b]->getX() == x && bases_[b]->getY() == y) return false;
        const std::vector<std::shared_ptr<Unit> >& units = bases_[b]->getUnits();
        for (size_t i = 0; i < units.size(); ++i) {
            if (units[i]->isAlive() && units[i]->getX() == x && units[i]->getY() == y) return false;
        }
    }
    return true;
}

std::shared_ptr<Unit> GameController::spawnUnit(Base& base, UnitType type, int x, int y) {
    bool ok = map_.inBounds(x, y) && isCellFree(x, y) && map_.getTerrain(x, y).isPassable(type);
    if (!ok) {
        logMessage("Game", "Нельзя создать " + toString(type) + " в клетке (" +
                           std::to_string(x) + "," + std::to_string(y) + ")");
        return std::shared_ptr<Unit>();
    }
    std::shared_ptr<Unit> unit = base.createUnit(type, x, y);
    return unit;
}

bool GameController::moveUnit(Unit& unit, int dx, int dy) {
    std::string who = unit.getType() + "#" + std::to_string(unit.getId());
    if (!unit.isAlive()) return false;
    if (std::abs(dx) > 1 || std::abs(dy) > 1 || (dx == 0 && dy == 0)) {
        logMessage("Game", who + ": можно сделать только один шаг на соседнюю клетку");
        return false;
    }
    if (unit.isStuck()) {
        logMessage("Game", who + " застрял и пропускает ход");
        return false;
    }
    int newX = unit.getX() + dx;
    int newY = unit.getY() + dy;
    if (!map_.inBounds(newX, newY)) {
        logMessage("Game", who + ": выход за границы карты");
        return false;
    }

    // Ландшафт проверяем через прокси.
    const Terrain& terrain = map_.getTerrain(newX, newY);
    if (!terrain.isPassable(unit.getKind())) return false;   // причину записал прокси
    double modifier = terrain.getSpeedModifier(unit.getKind());
    if (modifier <= 0.0) return false;

    // Чем медленнее клетка, тем больше очков нужно на шаг.
    double cost = 1.0 / modifier;
    bool enoughPoints = cost <= unit.getMovePoints() + 0.0001;
    bool freshTurn = unit.getMovePoints() >= unit.getSpeed();
    if (!enoughPoints && !freshTurn) {
        logMessage("Game", who + ": не хватает очков движения (нужно " + toText(cost) +
                           ", осталось " + toText(unit.getMovePoints()) + ") - клетка \"" + terrain.getName() + "\"");
        return false;
    }
    if (!isCellFree(newX, newY)) {
        logMessage("Game", who + ": клетка занята");
        return false;
    }

    int oldX = unit.getX();
    int oldY = unit.getY();
    unit.setPosition(newX, newY);
    unit.spendMovePoints(cost);
    logMessage("Game", who + " -> (" + std::to_string(newX) + "," + std::to_string(newY) + ") " +
                       terrain.getName() + ", цена шага " + toText(cost) +
                       ", осталось очков " + toText(unit.getMovePoints()));

    // Болото: есть шанс застрять.
    double stuckChance = terrain.getStuckChance(unit.getKind());
    if (stuckChance > 0.0 && nextRandom() < stuckChance * 100) {
        unit.stick(1);
        logMessage("Game", who + " застрял в болоте (пропустит следующий ход)");
    }
    return true;
}

bool GameController::attackUnit(Unit& attacker, Unit& target) {
    if (!attacker.isAlive() || !target.isAlive()) return false;
    if (attacker.getOwner() == target.getOwner()) return false;   // по своим не бьём
    int dist = distanceBetween(attacker.getX(), attacker.getY(), target.getX(), target.getY());
    if (dist > attacker.getEffectiveRange()) {
        logMessage("Game", attacker.getType() + ": цель слишком далеко");
        return false;
    }
    // Учитываем защиту клетки и базы.
    double bonus = map_.getTerrain(target.getX(), target.getY()).getDefenseBonus(target.getKind());
    if (target.getOwner() != nullptr) {
        bonus = bonus + target.getOwner()->defenseBonusAt(target.getX(), target.getY());
    }
    int before = target.getHealth();
    attacker.attackTarget(target, bonus);
    logMessage("Game", attacker.getType() + "#" + std::to_string(attacker.getId()) + " бьёт " +
        target.getType() + "#" + std::to_string(target.getId()) +
        " (атака " + std::to_string(attacker.getAttackValue()) + ", броня " + std::to_string(target.getArmorValue()) +
        ", бонус клетки " + toText(bonus) + ") -> урон " + std::to_string(before - target.getHealth()));
    return true;
}

bool GameController::attackBase(Unit& attacker, Base& base) {
    if (!attacker.isAlive() || base.isDestroyed()) return false;
    int dist = distanceBetween(attacker.getX(), attacker.getY(), base.getX(), base.getY());
    if (dist > attacker.getEffectiveRange()) {
        logMessage("Game", attacker.getType() + ": база слишком далеко");
        return false;
    }
    base.takeDamage(attacker.getAttackValue());
    return true;
}

// Выбираем оператор для объекта.
bool GameController::interactWithObject(Unit& unit) {
    NeutralObject* object = map_.getNeutralObjectAt(unit.getX(), unit.getY());
    if (object == nullptr) return false;

    TreasureChest* chest = dynamic_cast<TreasureChest*>(object);
    if (chest != nullptr) { unit + *chest; return true; }

    Trap* trap = dynamic_cast<Trap*>(object);
    if (trap != nullptr) { unit - *trap; return true; }

    Altar* altar = dynamic_cast<Altar*>(object);
    if (altar != nullptr) { unit * *altar; return true; }

    EnergySource* source = dynamic_cast<EnergySource*>(object);
    if (source != nullptr) { unit / *source; return true; }

    return false;
}

void GameController::endTurn() {
    for (size_t b = 0; b < bases_.size(); ++b) {
        const std::vector<std::shared_ptr<Unit> >& units = bases_[b]->getUnits();
        for (size_t i = 0; i < units.size(); ++i) units[i]->endTurn();
        bases_[b]->purgeDead();
    }
    turn_ = turn_ + 1;
}

// Сначала проверяем юнитов, потом базу, объект и ландшафт.
char GameController::getCellSymbol(int x, int y) const {
    for (size_t b = 0; b < bases_.size(); ++b) {
        const std::vector<std::shared_ptr<Unit> >& units = bases_[b]->getUnits();
        for (size_t i = 0; i < units.size(); ++i) {
            if (units[i]->isAlive() && units[i]->getX() == x && units[i]->getY() == y) {
                return units[i]->getSymbol();
            }
        }
    }
    for (size_t b = 0; b < bases_.size(); ++b) {
        if (!bases_[b]->isDestroyed() && bases_[b]->getX() == x && bases_[b]->getY() == y) {
            return bases_[b]->getSymbol();
        }
    }
    NeutralObject* object = map_.getNeutralObjectAt(x, y);
    if (object != nullptr && !object->isDepleted()) return object->getSymbol();
    return map_.getTerrain(x, y).getSymbol();
}

// Печать всей карты.
void GameController::printMap(std::ostream& os) const {
    int w = map_.getWidth();
    int h = map_.getHeight();
    os << "    ";
    for (int x = 0; x < w; ++x) os << (x % 10) << ' ';
    os << "\n  X ";
    for (int x = 0; x < w; ++x) os << "X ";
    os << "X\n";
    for (int y = 0; y < h; ++y) {
        if (y < 10) os << ' ';
        os << y << "X ";
        for (int x = 0; x < w; ++x) os << getCellSymbol(x, y) << ' ';
        os << "X\n";
    }
    os << "  X ";
    for (int x = 0; x < w; ++x) os << "X ";
    os << "X\n";
}

