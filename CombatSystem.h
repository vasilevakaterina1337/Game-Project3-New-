#pragma once
#include "Unit.h"
#include <algorithm>

class CombatSystem {
public:
    int calculateDamage(const Unit& attacker, const Unit& target) const {
        int raw = attacker.getAttack();
        int dmg = std::max(0, raw - target.getArmor());
        return dmg;
    }
};