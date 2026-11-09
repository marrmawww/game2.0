#pragma once

class Health {
public:
    explicit Health(int value) {
        if (value < 0) value = 0;
        value_ = value;
        max_ = value;
    }

    void decrease(int amount) {
        value_ -= amount;
        if (value_ < 0) value_ = 0;
    }

    void increase(int amount) {
        value_ += amount;
        if (value_ > max_) value_ = max_;
    }

    bool isZero() const { return value_ == 0; }
    int getValue() const { return value_; }
    int getMax() const { return max_; }

private:
    int value_;
    int max_;
};
