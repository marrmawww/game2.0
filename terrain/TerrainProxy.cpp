#include "TerrainProxy.h"
#include "Logger.h"

TerrainProxy::TerrainProxy(const Terrain* real) {
    real_ = real;
    hits_ = 0;
    misses_ = 0;
    bypasses_ = 0;
    for (int m = 0; m < 4; ++m) {
        for (int k = 0; k < KIND_COUNT; ++k) {
            cache_[m][k] = 0.0;
            known_[m][k] = false;
        }
    }
}

double TerrainProxy::getFromCache(int method, UnitType type) const {
    int k = static_cast<int>(type);
    if (known_[method][k]) {   
        hits_ = hits_ + 1;
        return cache_[method][k];
    }
    misses_ = misses_ + 1;    
    double value = 0.0;
    if (method == 0) {
        if (real_->isPassable(type)) value = 1.0;
    } else if (method == 1) {
        value = real_->getSpeedModifier(type);
    } else if (method == 2) {
        value = real_->getDefenseBonus(type);
    } else {
        value = real_->getStuckChance(type);
    }
    cache_[method][k] = value;
    known_[method][k] = true;
    return value;
}

bool TerrainProxy::isPassable(UnitType type) const {
    if (isFlying(type)) {  
        bypasses_ = bypasses_ + 1;
        logMessage("Proxy", toString(type) + " летит над клеткой \"" + real_->getName() + "\"");
        return true;
    }
    bool ok = getFromCache(0, type) > 0.5;
    if (!ok) {
        logMessage("Proxy", "Запрет: " + toString(type) + " не может пройти по \"" + real_->getName() + "\"");
    }
    return ok;
}

double TerrainProxy::getSpeedModifier(UnitType type) const {
    if (isFlying(type)) return 1.0;
    return getFromCache(1, type);
}

double TerrainProxy::getDefenseBonus(UnitType type) const {
    if (isFlying(type)) return 0.0;
    return getFromCache(2, type);
}

double TerrainProxy::getStuckChance(UnitType type) const {
    if (isFlying(type)) return 0.0;
    return getFromCache(3, type);
}
