#pragma once
#include "Unit.h"

class Archer : public Unit {
public:
    std::string getCategory() const override { return "Archer"; }
    int getRange() const override { return range_; }

protected:
    Archer(const Health& h, const Armor& a, const Attack& at,
        int speed, int range, int x, int y)
        : Unit(h, a, at, speed, x, y) {
        range_ = range;
    }

private:
    int range_;
};

class LightArcher final : public Archer {
public:
    LightArcher(int x = 0, int y = 0)
        : Archer(Health(60), Armor(1), Attack(12), 2, 5, x, y) {
    }

    std::unique_ptr<Unit> clone() const override {
        return std::unique_ptr<Unit>(new LightArcher(*this));
    }
    std::string getType() const override { return "LightArcher"; }
    UnitKind getKind() const override { return UnitKind::LightArcher; }
    char getSymbol() const override { return 'a'; }
};


class HeavyArcher final : public Archer {
public:
    HeavyArcher(int x = 0, int y = 0)
        : Archer(Health(80), Armor(3), Attack(20), 1, 7, x, y) {
    }

    std::unique_ptr<Unit> clone() const override {
        return std::unique_ptr<Unit>(new HeavyArcher(*this));
    }
    std::string getType() const override { return "HeavyArcher"; }
    UnitKind getKind() const override { return UnitKind::HeavyArcher; }
    char getSymbol() const override { return 'A'; }
};
