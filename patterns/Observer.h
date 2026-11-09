#pragma once
#include <string>
#include <vector>



enum class UnitEventType { Created, Damaged, Destroyed };


inline long long nextEventTimestamp() {
    static long long counter = 0;
    counter = counter + 1;
    return counter;
}


struct UnitEvent {
    UnitEventType type;
    int unitId;
    std::string unitName;
    int damage;
    long long timestamp;

    UnitEvent(UnitEventType t, int id, const std::string& name, int dmg) {
        type = t;
        unitId = id;
        unitName = name;
        damage = dmg;
        timestamp = nextEventTimestamp();
    }
};

class IObserver {
public:
    virtual ~IObserver() {}
    virtual void onUnitEvent(const UnitEvent& event) = 0;
};


class IObservable {
public:
    IObservable() {}

    IObservable(const IObservable&) {}

    virtual ~IObservable() {}

    void subscribe(IObserver* observer) {
        for (size_t i = 0; i < observers_.size(); ++i) {
            if (observers_[i] == observer) return; 
        }
        observers_.push_back(observer);
    }

    void unsubscribe(IObserver* observer) {
        for (size_t i = 0; i < observers_.size(); ++i) {
            if (observers_[i] == observer) {
                observers_.erase(observers_.begin() + i);
                return;
            }
        }
    }

    void notify(const UnitEvent& event) const {
        for (size_t i = 0; i < observers_.size(); ++i) {
            observers_[i]->onUnitEvent(event);
        }
    }

    size_t observerCount() const { return observers_.size(); }

private:
    std::vector<IObserver*> observers_;
};
