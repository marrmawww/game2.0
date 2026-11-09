#pragma once
#include "Unit.h"

class Cavalry : public Unit {
public:
    std::string getCategory() const override { return "Cavalry"; }

protected:
    Cavalry(const Health& h, const Armor& a, const Attack& at, int speed, int x, int y)
        : Unit(h, a, at, speed, x, y) {
    }
};


class Cataphract final : public Cavalry {
public:
    Cataphract(int x = 0, int y = 0)
        : Cavalry(Health(150), Armor(10), Attack(25), 3, x, y) {
    }

    std::unique_ptr<Unit> clone() const override {
        return std::unique_ptr<Unit>(new Cataphract(*this));
    }
    std::string getType() const override { return "Cataphract"; }
    UnitKind getKind() const override { return UnitKind::Cataphract; }
    char getSymbol() const override { return 'C'; }
};

class HorseArcher final : public Cavalry {
public:
    HorseArcher(int x = 0, int y = 0)
        : Cavalry(Health(90), Armor(2), Attack(15), 4, x, y) {
    }

    std::unique_ptr<Unit> clone() const override {
        return std::unique_ptr<Unit>(new HorseArcher(*this));
    }
    std::string getType() const override { return "HorseArcher"; }
    UnitKind getKind() const override { return UnitKind::HorseArcher; }
    char getSymbol() const override { return 'H'; }
};
