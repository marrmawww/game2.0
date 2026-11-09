#pragma once
#include "Unit.h"


class Infantry : public Unit {
public:
    std::string getCategory() const override { return "Infantry"; }

protected:

    Infantry(const Health& h, const Armor& a, const Attack& at, int speed, int x, int y)
        : Unit(h, a, at, speed, x, y) {
    }
};


class Swordsman final : public Infantry {
public:
   
    Swordsman(int x = 0, int y = 0)
        : Infantry(Health(100), Armor(5), Attack(15), 2, x, y) {
    }

    std::unique_ptr<Unit> clone() const override {
        return std::unique_ptr<Unit>(new Swordsman(*this));
    }
    std::string getType() const override { return "Swordsman"; }
    UnitKind getKind() const override { return UnitKind::Swordsman; }
    char getSymbol() const override { return 'S'; }
};


class Spearman final : public Infantry {
public:
    Spearman(int x = 0, int y = 0)
        : Infantry(Health(90), Armor(6), Attack(14), 2, x, y) {
    }

    std::unique_ptr<Unit> clone() const override {
        return std::unique_ptr<Unit>(new Spearman(*this));
    }
    std::string getType() const override { return "Spearman"; }
    UnitKind getKind() const override { return UnitKind::Spearman; }
    char getSymbol() const override { return 'P'; }
};
