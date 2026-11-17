#pragma once
#include <algorithm>

class Health {
    int value;
    int maxValue;
public:
    Health(int maxV) : value(maxV), maxValue(maxV) {}
    void decrease(int amount) { 
        value = std::max(0, value - amount);
    }
    void increase(int amount) {
        value = std::min(maxValue, value + amount); 
    }
    bool isZero() const { 
        return value <= 0; 
    }
    int getValue() const { 
        return value; 
    }
};

class Armor {
    int value;
public:
    Armor(int v) : value(v) {}
    int reduceDamage(int damage) const { 
        return std::max(0, damage - value); 
    }
    int getValue() const { 
        return value; 
    }
};

class Attack {
    int value;
public:
    Attack(int v) : value(v) {}
    int calculateDamage(const Armor& targetArmor) const { 
        return targetArmor.reduceDamage(value); 
    }
    int getValue() const { 
        return value; 
    }
};
