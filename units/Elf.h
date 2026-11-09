#pragma once
#include "Unit.h"


class Elf : public Unit {
public:
    std::string getCategory() const override { return "Elf"; }

protected:
    Elf(const Health& h, const Armor& a, const Attack& at,
        int speed, int x, int y)
        : Unit(h, a, at, speed, x, y) {}
};


class ElfRanger : public Elf {
public:
    ElfRanger(int x = 0, int y = 0)
        : Elf(Health(85), Armor(3), Attack(19), 3, x, y) {
    }
    std::unique_ptr<Unit> clone() const override {
        return std::unique_ptr<Unit>(new ElfRanger(*this));
    }
    std::string getType() const override { return "ElfRanger"; }
    UnitKind getKind() const override { return UnitKind::ElfRanger; }
    char getSymbol() const override { return 'E'; }
};


class ElfDruid : public Elf {
public:
    ElfDruid(int x = 0, int y = 0)
        : Elf(Health(110), Armor(5), Attack(16), 2, x, y) {
    }
    std::unique_ptr<Unit> clone() const override {
        return std::unique_ptr<Unit>(new ElfDruid(*this));
    }
    std::string getType() const override { return "ElfDruid"; }
    UnitKind getKind() const override { return UnitKind::ElfDruid; }
    char getSymbol() const override { return 'D'; }
};


class GryphonRider : public Elf {
public:
    GryphonRider(int x = 0, int y = 0)
        : Elf(Health(90), Armor(3), Attack(17), 4, x, y) {
    }
    std::unique_ptr<Unit> clone() const override {
        return std::unique_ptr<Unit>(new GryphonRider(*this));
    }
    std::string getType() const override { return "GryphonRider"; }
    UnitKind getKind() const override { return UnitKind::GryphonRider; }
    char getSymbol() const override { return 'G'; }
};
