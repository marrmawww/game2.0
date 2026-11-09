#include "Unit.h"
#include <cstdlib>
#include <sstream>
#include <stdexcept>


static int nextUnitId() {
    static int counter = 0;
    counter = counter + 1;
    return counter;
}

Unit::Unit(const Health& health, const Armor& armor, const Attack& attack,
    int speed, int x, int y)
    : health_(health), armor_(armor), attack_(attack),
      speed_(speed), x_(x), y_(y) {
    if (speed < 0) throw std::invalid_argument("Юнит: скорость не может быть меньше 0");
    id_ = nextUnitId();
    movePoints_ = speed;
    stuckTurns_ = 0;
    destroyedSent_ = false;
    owner_ = nullptr;
}

Unit::Unit(const Unit& other)
    : IObservable(),
      health_(other.health_), armor_(other.armor_), attack_(other.attack_),
      speed_(other.speed_), x_(other.x_), y_(other.y_) {
    id_ = nextUnitId();
    movePoints_ = other.movePoints_;
    attackBuff_ = other.attackBuff_;
    speedBuff_ = other.speedBuff_;
    rangeBuff_ = other.rangeBuff_;
    stuckTurns_ = other.stuckTurns_;
    destroyedSent_ = other.destroyedSent_;
    owner_ = nullptr;
}

void Unit::spendMovePoints(double cost) {
    movePoints_ = movePoints_ - cost;
    if (movePoints_ < 0) movePoints_ = 0;
}

void Unit::sendEvent(UnitEventType type, int damage) const {
    UnitEvent event(type, id_, getType(), damage);
    notify(event);
}

void Unit::announceCreated() {
    sendEvent(UnitEventType::Created, 0);
}


void Unit::applyDamage(int finalDamage) {
    if (finalDamage <= 0) return;
    if (!isAlive()) return;
    health_.decrease(finalDamage);
    sendEvent(UnitEventType::Damaged, finalDamage);
    if (!isAlive() && !destroyedSent_) {
        destroyedSent_ = true;   // о гибели сообщаем только один раз
        sendEvent(UnitEventType::Destroyed, 0);
    }
}

void Unit::takeDamage(int rawDamage, double defenseBonus) {
    int afterArmor = armor_.reduceDamage(rawDamage);
    int finalDamage = afterArmor - static_cast<int>(defenseBonus);
    if (finalDamage < 0) finalDamage = 0;
    applyDamage(finalDamage);
}

void Unit::takeTrueDamage(int amount) {
    if (amount < 0) throw std::invalid_argument("Урон не может быть меньше 0");
    applyDamage(amount);
}

void Unit::attackTarget(Unit& target, double targetDefenseBonus) const {
    target.takeDamage(getAttackValue(), targetDefenseBonus);
}

void Unit::heal(int amount) {
    health_.increase(amount);
}


void Unit::endTurn() {
    if (attackBuff_.turns > 0) {
        attackBuff_.turns = attackBuff_.turns - 1;
        if (attackBuff_.turns == 0) attackBuff_.value = 0;
    }
    if (speedBuff_.turns > 0) {
        speedBuff_.turns = speedBuff_.turns - 1;
        if (speedBuff_.turns == 0) speedBuff_.value = 0;
    }
    if (rangeBuff_.turns > 0) {
        rangeBuff_.turns = rangeBuff_.turns - 1;
        if (rangeBuff_.turns == 0) rangeBuff_.value = 0;
    }
    if (stuckTurns_ > 0) stuckTurns_ = stuckTurns_ - 1;
    movePoints_ = getSpeed();  
}

std::string Unit::describe() const {
    std::ostringstream out;
    out << getType() << "#" << id_ << " [" << getCategory() << "]"
        << " (" << x_ << "," << y_ << ")"
        << " ЗД " << health_.getValue() << "/" << health_.getMax()
        << " БРО " << armor_.getValue()
        << " АТК " << getAttackValue()
        << " СКР " << getSpeed();
    return out.str();
}
