#pragma once
#include "Unit.h"

class UnitFactory {
public:
    virtual std::unique_ptr<Unit> createUnit(int id) const = 0;
    virtual ~UnitFactory() = default;
};

class InfantryFactory : public UnitFactory {
public:
    std::unique_ptr<Unit> createUnit(int id) const override { 
        return std::make_unique<Infantry>(id, 100, 10, 15); 
    }
};
class ArcherFactory : public UnitFactory {
public:
    std::unique_ptr<Unit> createUnit(int id) const override { 
        return std::make_unique<Archer>(id, 70, 5, 25); 
    }
};
class CavalryFactory : public UnitFactory {
public:
    std::unique_ptr<Unit> createUnit(int id) const override { 
        return std::make_unique<Cavalry>(id, 120, 15, 20); 
    }
};
class WorkerFactory : public UnitFactory {
public:
    std::unique_ptr<Unit> createUnit(int id) const override { 
        return std::make_unique<Worker>(id, 50, 0, 5); 
    }
};
class ScoutFactory : public UnitFactory {
public:
    std::unique_ptr<Unit> createUnit(int id) const override { 
        return std::make_unique<Scout>(id, 60, 5, 10); 
    }
};
class TankFactory : public UnitFactory {
public:
    std::unique_ptr<Unit> createUnit(int id) const override { 
        return std::make_unique<Tank>(id, 300, 50, 40); 
    }
};
class MedicFactory : public UnitFactory {
public:
    std::unique_ptr<Unit> createUnit(int id) const override { 
        return std::make_unique<Medic>(id, 80, 5, 5); 
    }
};