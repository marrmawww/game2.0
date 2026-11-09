#pragma once
#include "Terrain.h"


class Mountains : public Terrain {
public:
    bool isPassable(UnitType type) const override { return isFlying(type) || isMountaineer(type); }

    double getSpeedModifier(UnitType type) const override {
        if (isFlying(type)) return 1.0;
        if (isMountaineer(type)) return 0.5;
        return 0.0;   
    }

    double getDefenseBonus(UnitType type) const override { return isMountaineer(type) ? 3.0 : 0.0; }

    std::string getName() const override { return "Горы"; }
    char getSymbol() const override { return '^'; }
    std::unique_ptr<Terrain> clone() const override { return std::unique_ptr<Terrain>(new Mountains(*this)); }
};
