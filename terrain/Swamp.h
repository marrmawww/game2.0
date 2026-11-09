#pragma once
#include "Terrain.h"


class Swamp : public Terrain {
public:
    bool isPassable(UnitType) const override { return true; }
    double getSpeedModifier(UnitType type) const override { return isFlying(type) ? 1.0 : 0.5; }
    double getDefenseBonus(UnitType) const override { return 0.0; }

    double getStuckChance(UnitType type) const override {
        if (isFlying(type)) return 0.0;
        return categoryOf(type) == UnitCategory::Elf ? 0.1 : 0.4;
    }

    std::string getName() const override { return "Болото"; }
    char getSymbol() const override { return '~'; }
    std::unique_ptr<Terrain> clone() const override { return std::unique_ptr<Terrain>(new Swamp(*this)); }
};
