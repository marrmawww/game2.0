#pragma once
#include "InteractionStrategy.h"
#include "Resources.h"


class AttackBoostStrategy : public IInteractionStrategy {
public:
    AttackBoostStrategy(int bonus, int turns) : bonus_(bonus), turns_(turns) {}
    void apply(Unit& unit) override;
    std::string name() const override { return "AttackBoost"; }
private:
    int bonus_;
    int turns_;
};


class RangeBoostStrategy : public IInteractionStrategy {
public:
    RangeBoostStrategy(int bonus, int turns) : bonus_(bonus), turns_(turns) {}
    void apply(Unit& unit) override;
    std::string name() const override { return "RangeBoost"; }
private:
    int bonus_;
    int turns_;
};


class SpeedBoostStrategy : public IInteractionStrategy {
public:
    SpeedBoostStrategy(int bonus, int turns) : bonus_(bonus), turns_(turns) {}
    void apply(Unit& unit) override;
    std::string name() const override { return "SpeedBoost"; }
private:
    int bonus_;
    int turns_;
};


class HealthRestoreStrategy : public IInteractionStrategy {
public:
    void apply(Unit& unit) override;
    std::string name() const override { return "HealthRestore"; }
};


class DamageStrategy : public IInteractionStrategy {
public:
    explicit DamageStrategy(int damage) : damage_(damage) {}
    void apply(Unit& unit) override;
    std::string name() const override { return "Damage"; }
private:
    int damage_;
};


class ResourceStrategy : public IInteractionStrategy {
public:
    explicit ResourceStrategy(const Resources& loot) : loot_(loot) {}
    void apply(Unit& unit) override;
    std::string name() const override { return "Resource"; }
private:
    Resources loot_;
};
