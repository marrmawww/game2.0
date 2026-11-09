#include "UnitFactory.h"
#include <stdexcept>
#include "Infantry.h"
#include "Archer.h"
#include "Cavalry.h"
#include "Elf.h"



std::unique_ptr<Unit> InfantryFactory::createUnit(UnitKind kind, int x, int y) const {
    if (kind == UnitKind::Swordsman) return std::unique_ptr<Unit>(new Swordsman(x, y));
    if (kind == UnitKind::Spearman)  return std::unique_ptr<Unit>(new Spearman(x, y));
    throw std::invalid_argument(name() + " не умеет создавать " + toString(kind));
}

std::unique_ptr<Unit> ArcherFactory::createUnit(UnitKind kind, int x, int y) const {
    if (kind == UnitKind::LightArcher) return std::unique_ptr<Unit>(new LightArcher(x, y));
    if (kind == UnitKind::HeavyArcher) return std::unique_ptr<Unit>(new HeavyArcher(x, y));
    throw std::invalid_argument(name() + " не умеет создавать " + toString(kind));
}

std::unique_ptr<Unit> CavalryFactory::createUnit(UnitKind kind, int x, int y) const {
    if (kind == UnitKind::Cataphract)  return std::unique_ptr<Unit>(new Cataphract(x, y));
    if (kind == UnitKind::HorseArcher) return std::unique_ptr<Unit>(new HorseArcher(x, y));
    throw std::invalid_argument(name() + " не умеет создавать " + toString(kind));
}



std::unique_ptr<Unit> ElfFactory::createUnit(UnitKind kind, int x, int y) const {
    if (kind == UnitKind::ElfRanger) return std::unique_ptr<Unit>(new ElfRanger(x, y));
    if (kind == UnitKind::ElfDruid)  return std::unique_ptr<Unit>(new ElfDruid(x, y));
    if (kind == UnitKind::GryphonRider) return std::unique_ptr<Unit>(new GryphonRider(x, y));
    throw std::invalid_argument(name() + " не умеет создавать " + toString(kind));
}

const UnitFactory& factoryFor(UnitKind kind) {
    static InfantryFactory infantry;
    static ArcherFactory archer;
    static CavalryFactory cavalry;
    static ElfFactory elf;
    UnitCategory category = categoryOf(kind);
    if (category == UnitCategory::Infantry) return infantry;
    if (category == UnitCategory::Archer)   return archer;
    if (category == UnitCategory::Cavalry)  return cavalry;
    return elf;
}
