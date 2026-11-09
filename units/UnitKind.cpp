#include "UnitKind.h"

std::string toString(UnitKind kind) {
    if (kind == UnitKind::Swordsman)   return "Мечник";
    if (kind == UnitKind::Spearman)    return "Копейщик";
    if (kind == UnitKind::LightArcher) return "Лёгкий лучник";
    if (kind == UnitKind::HeavyArcher) return "Тяжёлый лучник";
    if (kind == UnitKind::Cataphract)  return "Катафракт";
    if (kind == UnitKind::HorseArcher) return "Конный лучник";
    if (kind == UnitKind::ElfRanger)  return "Эльфийский следопыт";
    if (kind == UnitKind::ElfDruid)   return "Эльфийский друид";
    if (kind == UnitKind::GryphonRider) return "Наездник на грифоне";
    return "Неизвестный";
}
