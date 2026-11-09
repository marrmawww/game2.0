#include "Strategies.h"
#include "Base.h"
#include "Logger.h"
#include "Unit.h"

void AttackBoostStrategy::apply(Unit& unit) {
    unit.addAttackBuff(bonus_, turns_);
    logMessage("Strategy:AttackBoost", unit.getType() + ": атака +" + std::to_string(bonus_) +
        " на " + std::to_string(turns_) + " хода (теперь " + std::to_string(unit.getAttackValue()) + ")");
}

void RangeBoostStrategy::apply(Unit& unit) {
    unit.addRangeBuff(bonus_, turns_);
    logMessage("Strategy:RangeBoost", unit.getType() + ": дальность +" + std::to_string(bonus_) +
        " на " + std::to_string(turns_) + " хода (теперь " + std::to_string(unit.getEffectiveRange()) + ")");
}

void SpeedBoostStrategy::apply(Unit& unit) {
    unit.addSpeedBuff(bonus_, turns_);
    logMessage("Strategy:SpeedBoost", unit.getType() + ": скорость +" + std::to_string(bonus_) +
        " на " + std::to_string(turns_) + " хода (теперь " + std::to_string(unit.getSpeed()) + ")");
}


void HealthRestoreStrategy::apply(Unit& unit) {
    unit.heal(unit.getMaxHealth());
    logMessage("Strategy:HealthRestore", unit.getType() + ": здоровье восстановлено до " +
        std::to_string(unit.getHealth()) + "/" + std::to_string(unit.getMaxHealth()));
}

void DamageStrategy::apply(Unit& unit) {
    unit.takeTrueDamage(damage_);
    logMessage("Strategy:Damage", unit.getType() + ": получил " + std::to_string(damage_) +
        " урона от ловушки (здоровье " + std::to_string(unit.getHealth()) + ")");
}

void ResourceStrategy::apply(Unit& unit) {
    Base* base = unit.getOwner();
    if (base == nullptr) {
        logMessage("Strategy:Resource", unit.getType() + ": у юнита нет базы, ресурсы потеряны");
        return;
    }
    base->addResources(loot_);
    logMessage("Strategy:Resource", unit.getType() + " принёс на базу: " + loot_.toString());
}
