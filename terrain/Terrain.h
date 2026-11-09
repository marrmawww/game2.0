#pragma once
#include <memory>
#include <string>
#include "UnitKind.h"


class Terrain {
public:
    virtual ~Terrain() {}

    virtual bool isPassable(UnitType type) const = 0;
    virtual double getSpeedModifier(UnitType type) const = 0;
    virtual double getDefenseBonus(UnitType type) const = 0;
   
    virtual double getStuckChance(UnitType) const { return 0.0; }

    virtual std::string getName() const = 0;
    virtual char getSymbol() const = 0;
    virtual std::unique_ptr<Terrain> clone() const = 0;
};
