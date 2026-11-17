#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>
#include <string>
#include "Unit.h"
#include "Building.h" 

struct UnitEvent {
    enum Type { Created, Damaged, Destroyed } type;
    int unitId;
};

class IObserver {
public:
    virtual void onUnitEvent(const UnitEvent& event) = 0;
    virtual ~IObserver() = default;
};

class IObservable {
    std::vector<IObserver*> observers;
public:
    void subscribe(IObserver* obs) { 
        observers.push_back(obs); 
    }
    void notify(const UnitEvent& event) {
        for (auto obs : observers) 
            obs->onUnitEvent(event);
    }
};

class Base : public IObserver {
    int id;
    int health = 1000;
    size_t maxUnits = 10;
    std::vector<std::shared_ptr<Unit>> units;

public:

    int wood = 1000;
    int metal = 500;
    int energy = 200;
    std::vector<std::shared_ptr<Building>> buildings;

    Base(int id) : id(id) {}

    void onUnitEvent(const UnitEvent& event) override;
    void addUnit(std::shared_ptr<Unit> unit);
    int getId() const { 
        return id; 
    }
    size_t getUnitCount() const { 
        return units.size(); 
    }
    std::vector<std::shared_ptr<Unit>> getUnits() const { 
        return units; 
    }

    void addBuilding(std::shared_ptr<Building> b) {
        buildings.push_back(b);
    }

    void printStats() const {
        std::cout << "\nСтатистика\n";
        std::cout << "Ресурсы:\n";
        std::cout << "  Дерево: " << wood << "\n";
        std::cout << "  Металл: " << metal << "\n";
        std::cout << "  Энергия: " << energy << "\n";
        std::cout << "Юниты: " << units.size() << "\n";
        std::cout << "Здания: " << buildings.size() << "\n";
        for (const auto& b : buildings) {
            std::cout << "  - " << b->getType() << " (Прогресс: " << b->getProgress() << "%)\n";
        }
    }
};