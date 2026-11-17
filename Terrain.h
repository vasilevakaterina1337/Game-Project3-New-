#pragma once
#include <memory>
#include <iostream>
#include "Unit.h"

class Terrain {
public:
    virtual ~Terrain() = default;
    virtual bool isPassable(const Unit& unit) const = 0;
    virtual double getSpeedModifier(const Unit& unit) const = 0;
    virtual double getDefenseBonus(const Unit& unit) const = 0;
    virtual char getSymbol() const = 0;
};

class Plains : public Terrain {
public:
    bool isPassable(const Unit&) const override { 
        return true; 
    }
    double getSpeedModifier(const Unit&) const override { 
        return 1.0; 
    }
    double getDefenseBonus(const Unit&) const override { 
        return 0.0; 
    }
    char getSymbol() const override { 
        return '.'; 
    }
};

class Forest : public Terrain {
public:
    bool isPassable(const Unit& unit) const override { 
        return unit.getType() != UnitType::Cavalry; 
    }
    double getSpeedModifier(const Unit&) const override { 
        return 0.5; 
    }
    double getDefenseBonus(const Unit&) const override { 
        return 5.0; 
    }
    char getSymbol() const override { 
        return '*'; 
    }
};

class Swamp : public Terrain {
public:
    bool isPassable(const Unit&) const override { 
        return true; 
    }
    double getSpeedModifier(const Unit&) const override { 
        return 0.3; 
    }
    double getDefenseBonus(const Unit&) const override { 
        return 0.0; 
    }
    char getSymbol() const override { 
        return '~'; 
    }
};

class Mountains : public Terrain {
public:
    bool isPassable(const Unit& unit) const override { 
        return unit.getIsFlying(); 
    }
    double getSpeedModifier(const Unit&) const override { 
        return 0.5; 
    }
    double getDefenseBonus(const Unit&) const override { 
        return 10.0; 
    }
    char getSymbol() const override { 
        return '^'; 
    }
};

class TerrainProxy : public Terrain {
    std::shared_ptr<Terrain> realTerrain;
public:
    TerrainProxy(std::shared_ptr<Terrain> t) : realTerrain(t) {}
    bool isPassable(const Unit& unit) const override {
        bool passable = realTerrain->isPassable(unit);
        if (!passable) std::cout << "Юнит " << unit.getName() << " не может пройти\n";
        return passable;
    }
    double getSpeedModifier(const Unit& unit) const override { 
        return realTerrain->getSpeedModifier(unit); 
    }
    double getDefenseBonus(const Unit& unit) const override { 
        return realTerrain->getDefenseBonus(unit); 
    }
    char getSymbol() const override { 
        return realTerrain->getSymbol(); 
    } 
};