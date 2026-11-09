#pragma once
#include "Terrain.h"
#include "UnitKind.h"

class TerrainProxy : public Terrain {
public:
    explicit TerrainProxy(const Terrain* real);

    bool isPassable(UnitType type) const override;
    double getSpeedModifier(UnitType type) const override;
    double getDefenseBonus(UnitType type) const override;
    double getStuckChance(UnitType type) const override;

    std::string getName() const override { return real_->getName(); }
    char getSymbol() const override { return real_->getSymbol(); }
    std::unique_ptr<Terrain> clone() const override { return real_->clone(); }

    int cacheHits() const { return hits_; }
    int cacheMisses() const { return misses_; }
    int flyingBypasses() const { return bypasses_; }

private:
 
    double getFromCache(int method, UnitType type) const;

    const Terrain* real_;   

 
    mutable double cache_[4][KIND_COUNT];
    mutable bool known_[4][KIND_COUNT];
    mutable int hits_;
    mutable int misses_;
    mutable int bypasses_;
};
