#pragma once
#include "NeutralObject.h"
#include "Strategies.h"


class EnergySource : public NeutralObject {
public:
    EnergySource()
        : NeutralObject("Источник энергии", '+', -1, true,
                        std::shared_ptr<IInteractionStrategy>(new HealthRestoreStrategy())) {}

    void interact(Unit& unit) override {
        if (!tryUse(unit)) return;
        strategy_->apply(unit);
    }

    std::unique_ptr<NeutralObject> clone() const override {
        return std::unique_ptr<NeutralObject>(new EnergySource(*this));
    }
};
