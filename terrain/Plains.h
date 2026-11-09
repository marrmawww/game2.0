#pragma once
#include "Terrain.h"


class Plains : public Terrain {
public:
    bool isPassable(UnitType) const override { return true; }
    double getSpeedModifier(UnitType) const override { return 1.0; }
    double getDefenseBonus(UnitType) const override { return 0.0; }
    std::string getName() const override { return "Равнина"; }
    char getSymbol() const override { return '.'; }
    std::unique_ptr<Terrain> clone() const override { return std::unique_ptr<Terrain>(new Plains(*this)); }
};
