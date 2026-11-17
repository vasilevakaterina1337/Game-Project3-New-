#pragma once
#include <memory>
#include <iostream>
#include "Unit.h"

class InteractionStrategy {
public:
    virtual ~InteractionStrategy() = default;
    virtual void apply(Unit& unit) = 0;
};

class AttackBoostStrategy : public InteractionStrategy {
public:
    void apply(Unit& unit) override { 
        std::cout << "Атака усилена для " << unit.getName() << "\n"; 
    }
};

class HealthRestoreStrategy : public InteractionStrategy {
public:
    void apply(Unit& unit) override { 
        std::cout << "Здоровье восстановлено для " << unit.getName() << "\n"; 
        unit.takeDamage(-20); 
    }
};

class NeutralObject {
protected:
    std::shared_ptr<InteractionStrategy> strategy;
public:
    NeutralObject(std::shared_ptr<InteractionStrategy> strat) : strategy(strat) {}
    virtual ~NeutralObject() = default;

    virtual void interact(Unit& unit) {
        strategy->apply(unit); 
    }

    virtual NeutralObject& operator+(Unit& unit) { 
        interact(unit); 
        return *this; 
    }
    virtual NeutralObject& operator-(Unit& unit) {
        std::cout << "[Ловушка] " << unit.getName() << " получил 20 урона\n";
        unit.takeDamage(20);
        return *this;
    }
    virtual NeutralObject& operator*(Unit& unit) {
        std::cout << "[Алтарь] " << unit.getName() << " усилен\n";
        interact(unit);
        return *this;
    }
};

class TreasureChest : public NeutralObject {
public:
    TreasureChest() : NeutralObject(std::make_shared<HealthRestoreStrategy>()) {}
};

class Trap : public NeutralObject {
public:
    Trap() : NeutralObject(std::make_shared<AttackBoostStrategy>()) {}
};

class Altar : public NeutralObject {
public:
    Altar() : NeutralObject(std::make_shared<AttackBoostStrategy>()) {}
};
