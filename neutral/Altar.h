#pragma once
#include "NeutralObject.h"
#include "Strategies.h"


class Altar : public NeutralObject {
public:
    Altar()
        : NeutralObject("Алтарь силы", '!', -1, true, std::shared_ptr<IInteractionStrategy>()) {
        infantryStrategy_.reset(new AttackBoostStrategy(8, 3));
        archerStrategy_.reset(new RangeBoostStrategy(3, 3));
        cavalryStrategy_.reset(new SpeedBoostStrategy(2, 3));
        elfStrategy_.reset(new HealthRestoreStrategy());
    }

    std::shared_ptr<IInteractionStrategy> selectStrategy(const Unit& unit) const {
        UnitCategory category = categoryOf(unit.getKind());
        if (category == UnitCategory::Infantry) return infantryStrategy_;
        if (category == UnitCategory::Archer) return archerStrategy_;
        if (category == UnitCategory::Cavalry) return cavalryStrategy_;
        return elfStrategy_;
    }

    void interact(Unit& unit) override {
        if (!tryUse(unit)) return;
        strategy_ = selectStrategy(unit);
        logMessage("Neutral", "Алтарь выбрал стратегию " + strategy_->name() +
                              " для категории \"" + toString(categoryOf(unit.getKind())) + "\"");
        strategy_->apply(unit);
    }

    std::unique_ptr<NeutralObject> clone() const override {
        return std::unique_ptr<NeutralObject>(new Altar(*this));
    }

private:
    std::shared_ptr<IInteractionStrategy> infantryStrategy_;
    std::shared_ptr<IInteractionStrategy> archerStrategy_;
    std::shared_ptr<IInteractionStrategy> cavalryStrategy_;
    std::shared_ptr<IInteractionStrategy> elfStrategy_;
};
