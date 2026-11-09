#pragma once
#include <memory>
#include <string>
#include <vector>
#include "InteractionStrategy.h"
#include "Logger.h"
#include "Unit.h"

class NeutralObject {
public:

    NeutralObject(const std::string& name, char symbol, int uses, bool oncePerUnit,
                  const std::shared_ptr<IInteractionStrategy>& strategy) {
        name_ = name;
        symbol_ = symbol;
        usesLeft_ = uses;
        oncePerUnit_ = oncePerUnit;
        strategy_ = strategy;
    }
    virtual ~NeutralObject() {}

    virtual void interact(Unit& unit) = 0;
    virtual std::unique_ptr<NeutralObject> clone() const = 0;

    const std::string& getName() const { return name_; }
    char getSymbol() const { return symbol_; }
    bool isDepleted() const { return usesLeft_ == 0; }
    int getUsesLeft() const { return usesLeft_; }

    bool hasVisited(int unitId) const {
        for (size_t i = 0; i < visitors_.size(); ++i) {
            if (visitors_[i] == unitId) return true;
        }
        return false;
    }

protected:

    bool tryUse(const Unit& unit) {
        if (usesLeft_ == 0) {
            logMessage("Neutral", name_ + ": объект исчерпан");
            return false;
        }
        if (oncePerUnit_ && hasVisited(unit.getId())) {
            logMessage("Neutral", name_ + ": " + unit.getType() + "#" + std::to_string(unit.getId()) +
                                  " уже использовал этот объект");
            return false;
        }
        if (usesLeft_ > 0) usesLeft_ = usesLeft_ - 1;
        visitors_.push_back(unit.getId());
        return true;
    }

    std::string name_;
    char symbol_;
    int usesLeft_;
    bool oncePerUnit_;
    std::vector<int> visitors_;  
    std::shared_ptr<IInteractionStrategy> strategy_;
};
