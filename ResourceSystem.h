#pragma once
#include <iostream>
#include <string>

class ResourceSystem {
public:
    int wood = 1000;
    int metal = 500;
    int energy = 200;

    bool spendWood(int amount) {
        if (wood >= amount) { wood -= amount; return true; }
        std::cout << "[Ресурсы] Недостаточно дерева\n";
        return false;
    }
    bool spendMetal(int amount) {
        if (metal >= amount) { metal -= amount; return true; }
        std::cout << "[Ресурсы] Недостаточно металла\n";
        return false;
    }
    bool spendEnergy(int amount) {
        if (energy >= amount) { energy -= amount; return true; }
        std::cout << "[Ресурсы] Недостаточно энергии\n";
        return false;
    }
    void addWood(int a) { wood += a; }
    void addMetal(int a) { metal += a; }
    void addEnergy(int a) { energy += a; }

    bool distribute(const std::string& from, const std::string& to, int amount) {
        if (from == "wood") { if (!spendWood(amount)) return false; }
        else if (from == "metal") { if (!spendMetal(amount)) return false; }
        else if (from == "energy") { if (!spendEnergy(amount)) return false; }
        else { std::cout << "[Ресурсы] Неизвестный ресурс: " << from << "\n"; return false; }
        std::cout << "[Ресурсы] Передано " << amount << " " << from << " -> " << to << "\n";
        return true;
    }
};