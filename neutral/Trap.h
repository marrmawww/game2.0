#pragma once
#include "NeutralObject.h"
#include "Strategies.h"


class Trap : public NeutralObject {
public:
    Trap(int damage = 25)
        : NeutralObject("Ловушка", 'T', 1, false,
                        std::shared_ptr<IInteractionStrategy>(new DamageStrategy(damage))) {}

    void interact(Unit& unit) override {
        if (!tryUse(unit)) return;
        strategy_->apply(unit);
    }

    std::unique_ptr<NeutralObject> clone() const override {
        return std::unique_ptr<NeutralObject>(new Trap(*this));
    }
};
