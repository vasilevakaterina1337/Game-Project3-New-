#pragma once
#include "Unit.h"
#include "GameField.h"
#include <vector>
#include <memory>
#include <algorithm>

class UnitManager {
    GameField& field;
    std::vector<std::shared_ptr<Unit>> units;
    int nextId = 1;
public:
    UnitManager(GameField& f) : field(f) {}
    void addExisting(std::shared_ptr<Unit> u) {
        units.push_back(u);
    }
    int allocateId() {
        return nextId++;
    }

    std::shared_ptr<Unit> getUnitById(int id) const {
        for (auto& u : units) 
            if (u->getId() == id)
                return u;
        return nullptr;
    }

    const std::vector<std::shared_ptr<Unit>>& all() const { 
        return units;
    }

    void removeUnit(int id) {
        units.erase(std::remove_if(units.begin(), units.end(),
            [&](const std::shared_ptr<Unit>& u) { return u->getId() == id; }), units.end());
    }
};