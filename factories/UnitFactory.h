#pragma once
#include <memory>
#include <string>
#include "Unit.h"
#include "UnitKind.h"


class UnitFactory {
public:
    virtual ~UnitFactory() {}

    virtual std::unique_ptr<Unit> createUnit(UnitKind kind, int x, int y) const = 0;

    virtual std::string name() const = 0;
};


class InfantryFactory final : public UnitFactory {
public:
    std::unique_ptr<Unit> createUnit(UnitKind kind, int x, int y) const override;
    std::string name() const override { return "InfantryFactory"; }
};

class ArcherFactory final : public UnitFactory {
public:
    std::unique_ptr<Unit> createUnit(UnitKind kind, int x, int y) const override;
    std::string name() const override { return "ArcherFactory"; }
};


class CavalryFactory final : public UnitFactory {
public:
    std::unique_ptr<Unit> createUnit(UnitKind kind, int x, int y) const override;
    std::string name() const override { return "CavalryFactory"; }
};

class ElfFactory final : public UnitFactory {
public:
    std::unique_ptr<Unit> createUnit(UnitKind kind, int x, int y) const override;
    std::string name() const override { return "ElfFactory"; }
};


const UnitFactory& factoryFor(UnitKind kind);
