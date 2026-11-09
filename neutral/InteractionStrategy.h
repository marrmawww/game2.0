#pragma once
#include <string>

class Unit;

class IInteractionStrategy {
public:
    virtual ~IInteractionStrategy() {}
    virtual void apply(Unit& unit) = 0;
    virtual std::string name() const = 0;
};
