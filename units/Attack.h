#pragma once


class Attack {
public:
    explicit Attack(int value) {
        value_ = value < 0 ? 0 : value;
    }

    int getValue() const { return value_; }

private:
    int value_;
};
