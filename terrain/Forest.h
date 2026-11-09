#pragma once
#include "Terrain.h"


class Forest : public Terrain {
public:
    bool isPassable(UnitType type) const override { return !isHeavy(type); }

    double getSpeedModifier(UnitType type) const override {
        UnitCategory c = categoryOf(type);
        if (c == UnitCategory::Elf) return 1.0;      
        if (c == UnitCategory::Archer) return 0.75;
        return 0.5;                                   
    }

    double getDefenseBonus(UnitType type) const override {
        return categoryOf(type) == UnitCategory::Elf ? 4.0 : 2.0;
    }

    std::string getName() const override { return "Лес"; }
    char getSymbol() const override { return '*'; }
    std::unique_ptr<Terrain> clone() const override { return std::unique_ptr<Terrain>(new Forest(*this)); }
};
