#pragma once
#include <sstream>
#include <string>

// Запас ресурсов базы.
struct Resources {
    int gold;
    int wood;
    int metal;
    int energy;

    Resources() : gold(0), wood(0), metal(0), energy(0) {}
    Resources(int g, int w, int m, int e) : gold(g), wood(w), metal(m), energy(e) {}

    Resources& operator+=(const Resources& other) {
        gold += other.gold;
        wood += other.wood;
        metal += other.metal;
        energy += other.energy;
        return *this;
    }

    std::string toString() const {
        std::ostringstream out;
        out << "золото " << gold << ", дерево " << wood
            << ", металл " << metal << ", энергия " << energy;
        return out.str();
    }
};
