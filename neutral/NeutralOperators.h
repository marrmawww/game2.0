#pragma once
#include "Altar.h"
#include "EnergySource.h"
#include "Trap.h"
#include "TreasureChest.h"
#include "Unit.h"


inline Unit& operator+(Unit& unit, TreasureChest& chest)  { chest.interact(unit);  return unit; }  // собрать
inline Unit& operator-(Unit& unit, Trap& trap)            { trap.interact(unit);   return unit; }  // получить урон
inline Unit& operator*(Unit& unit, Altar& altar)          { altar.interact(unit);  return unit; }  // усилиться
inline Unit& operator/(Unit& unit, EnergySource& source)  { source.interact(unit); return unit; }  // исцелиться
