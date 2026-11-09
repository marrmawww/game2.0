#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Observer.h"
#include "Resources.h"
#include "Unit.h"
#include "UnitKind.h"

// Запись об уничтоженном юните (история базы).
struct DestroyedRecord {
    int unitId;
    std::string unitName;
    long long timestamp;
};

// База: создаёт юнитов, ведёт их учёт и НАБЛЮДАЕТ за ними (IObserver).
class Base : public IObserver {
public:
    Base(const std::string& name, char symbol, int x, int y,
         int health, size_t maxUnits, int defenseLevel,
         const Resources& resources = Resources());
    ~Base();

    // База копироваться и перемещаться не может: юниты хранят указатель на неё
    // (подписка и owner), поэтому копия сломала бы связи.
    Base(const Base&) = delete;
    Base& operator=(const Base&) = delete;
    Base(Base&&) = delete;
    Base& operator=(Base&&) = delete;

    // Создание через фабрику из ЛР №1. nullptr, если создать нельзя.
    std::shared_ptr<Unit> createUnit(UnitType type, int x, int y);
    bool removeUnit(int unitId);
    void purgeDead();   // убрать погибших из списка (без удаления из истории)

    // Реакция на события юнитов (Observer).
    void onUnitEvent(const UnitEvent& event) override;

    // Удар по базе: защита базы уменьшает урон.
    void takeDamage(int rawDamage);
    // Юниты возле базы (до 2 клеток) получают бонус защиты.
    int defenseBonusAt(int x, int y) const;

    void addResources(const Resources& r) { resources_ += r; }

    const std::string& getName() const { return name_; }
    char getSymbol() const { return symbol_; }
    int getX() const { return x_; }
    int getY() const { return y_; }
    int getHealth() const { return health_; }
    int getMaxHealth() const { return maxHealth_; }
    size_t getMaxUnits() const { return maxUnits_; }
    int getDefenseLevel() const { return defenseLevel_; }
    const Resources& getResources() const { return resources_; }
    bool isDestroyed() const { return health_ <= 0; }

    size_t getAliveCount() const { return aliveCount_; }
    const std::vector<std::shared_ptr<Unit> >& getUnits() const { return units_; }
    const std::vector<DestroyedRecord>& getDestroyedHistory() const { return history_; }

private:
    std::string name_;
    char symbol_;
    int x_;
    int y_;
    int health_;
    int maxHealth_;
    size_t maxUnits_;
    int defenseLevel_;
    Resources resources_;

    std::vector<std::shared_ptr<Unit> > units_;   // все юниты базы
    std::vector<DestroyedRecord> history_;
    size_t aliveCount_;                           // счётчик живых (меняет Observer)
};
