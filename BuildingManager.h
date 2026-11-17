#pragma once
#include "Building.h"
#include <vector>
#include <memory>

class BuildingManager {
    std::vector<std::shared_ptr<Building>> buildings;
public:
    void add(std::shared_ptr<Building> b) { buildings.push_back(b); }
    std::shared_ptr<Building> getById(int id) const {
        if (id >= 0 && id < (int)buildings.size()) return buildings[id];
        return nullptr;
    }
    const std::vector<std::shared_ptr<Building>>& all() const { return buildings; }
    void update() { for (auto& b : buildings) b->update(); }
};