#pragma once

class Armor {
public:
    explicit Armor(int value) {
        value_ = value < 0 ? 0 : value;
    }

    int reduceDamage(int damage) const {
        int result = damage - value_;
        return result < 0 ? 0 : result;
    }

    int getValue() const { return value_; }

private:
    int value_;
};
