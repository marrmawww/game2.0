#pragma once
#include <memory>
#include <string>
#include "Health.h"
#include "Armor.h"
#include "Attack.h"
#include "Observer.h"
#include "UnitKind.h"

class Base;   

struct Buff {
    int value;
    int turns;
    Buff() { value = 0; turns = 0; }
};


class Unit : public IObservable {
public:
    virtual ~Unit() {}

    virtual std::unique_ptr<Unit> clone() const = 0;
    virtual std::string getType() const = 0;
    virtual std::string getCategory() const = 0;
    virtual UnitKind getKind() const = 0;
    virtual char getSymbol() const = 0;
    virtual int getRange() const { return 1; }   

    int getId() const { return id_; }
    int getX() const { return x_; }
    int getY() const { return y_; }
    bool isAlive() const { return !health_.isZero(); }

  
    int getSpeed() const { return speed_ + speedBuff_.value; }
    int getAttackValue() const { return attack_.getValue() + attackBuff_.value; }
    int getEffectiveRange() const { return getRange() + rangeBuff_.value; }

    int getHealth() const { return health_.getValue(); }
    int getMaxHealth() const { return health_.getMax(); }
    int getArmorValue() const { return armor_.getValue(); }

    void setPosition(int x, int y) { x_ = x; y_ = y; }

    double getMovePoints() const { return movePoints_; }
    void spendMovePoints(double cost);

    // Урон с учётом брони и бонуса защиты (ландшафт, база).
    void takeDamage(int rawDamage, double defenseBonus = 0.0);
    // Урон, который броня не уменьшает (ловушка).
    void takeTrueDamage(int amount);
    void attackTarget(Unit& target, double targetDefenseBonus = 0.0) const;
    void heal(int amount);

    // временные бонусы от алтаря
    void addAttackBuff(int value, int turns) { attackBuff_.value = value; attackBuff_.turns = turns; }
    void addSpeedBuff(int value, int turns)  { speedBuff_.value = value;  speedBuff_.turns = turns; }
    void addRangeBuff(int value, int turns)  { rangeBuff_.value = value;  rangeBuff_.turns = turns; }

    // застревание в болоте
    void stick(int turns) { stuckTurns_ = turns; }
    bool isStuck() const { return stuckTurns_ > 0; }

    void endTurn();   // бонусы и застревание проходят на один ход

    Base* getOwner() const { return owner_; }
    void setOwner(Base* owner) { owner_ = owner; }


    void announceCreated();

    std::string describe() const;

protected:
    Unit(const Health& health, const Armor& armor, const Attack& attack,
        int speed, int x, int y);

    Unit(const Unit& other);

    Health health_;
    Armor armor_;
    Attack attack_;
    int speed_;
    int x_;
    int y_;

private:
    void applyDamage(int finalDamage);
    void sendEvent(UnitEventType type, int damage) const;

    int id_;
    double movePoints_;
    Buff attackBuff_;
    Buff speedBuff_;
    Buff rangeBuff_;
    int stuckTurns_;
    bool destroyedSent_;
    Base* owner_;
};
