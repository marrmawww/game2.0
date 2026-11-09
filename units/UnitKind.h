#pragma once
#include <string>


enum class UnitKind {
    Swordsman, Spearman,
    LightArcher, HeavyArcher,
    Cataphract, HorseArcher,
    ElfRanger, ElfDruid, GryphonRider
};

using UnitType = UnitKind;

const int KIND_COUNT = 9;

enum class UnitCategory { Infantry, Archer, Cavalry, Elf };

std::string toString(UnitKind kind);

inline std::string toString(UnitCategory category) {
    switch (category) {
    case UnitCategory::Infantry: return "Пехота";
    case UnitCategory::Archer:   return "Лучники";
    case UnitCategory::Cavalry:  return "Конница";
    case UnitCategory::Elf:      return "Эльфы";
    }
    return "?";
}

inline UnitCategory categoryOf(UnitKind kind) {
    switch (kind) {
    case UnitKind::Swordsman:
    case UnitKind::Spearman:     return UnitCategory::Infantry;
    case UnitKind::LightArcher:
    case UnitKind::HeavyArcher:  return UnitCategory::Archer;
    case UnitKind::Cataphract:
    case UnitKind::HorseArcher:  return UnitCategory::Cavalry;
    default:                     return UnitCategory::Elf;
    }
}

inline bool isFlying(UnitKind kind)     { return kind == UnitKind::GryphonRider; }
inline bool isHeavy(UnitKind kind)      { return kind == UnitKind::Cataphract; }
inline bool isMountaineer(UnitKind kind) { return kind == UnitKind::ElfRanger; }
