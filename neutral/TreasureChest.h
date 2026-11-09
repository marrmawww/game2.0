#pragma once
#include "NeutralObject.h"
#include "Strategies.h"


class TreasureChest : public NeutralObject {
public:
    TreasureChest(const Resources& loot = Resources(50, 20, 10, 0))
        : NeutralObject("Сундук", '$', 1, false,
                        std::shared_ptr<IInteractionStrategy>(new ResourceStrategy(loot))) {}

    void interact(Unit& unit) override {
        if (!tryUse(unit)) return;
        strategy_->apply(unit);
    }

    std::unique_ptr<NeutralObject> clone() const override {
        return std::unique_ptr<NeutralObject>(new TreasureChest(*this));
    }
};
