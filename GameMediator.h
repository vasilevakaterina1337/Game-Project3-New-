#pragma once
#include "GameField.h"
#include "Base.h"
#include "ResourceSystem.h"
#include "CombatSystem.h"
#include "UnitManager.h"
#include "BuildingManager.h"
#include "Building.h"
#include <vector>
#include <string>

class GameMediator {
    GameField& field;
    Base& base;
    std::shared_ptr<IObservable> eventBus;

    UnitManager unitMgr;
    BuildingManager buildMgr;
    ResourceSystem resources;
    CombatSystem combat;

    int researchRemaining = 0;
    std::string researchName;

public:
    GameMediator(GameField& f, std::shared_ptr<IObservable> bus, Base& b);

    void setBuilding(int x, int y, std::shared_ptr<Building> b);
    void addUnit(std::shared_ptr<Unit> unit, int x, int y);
    void moveUnit(int unitId, int x, int y);
    void attackUnit(int attackerId, int targetId);
    void interactWithObject(int unitId, int x, int y);
    void setTerrain(int x, int y, std::shared_ptr<Terrain> t);
    void setObject(int x, int y, std::shared_ptr<NeutralObject> o);
    std::shared_ptr<Unit> findUnit(int id) const;

    void createUnit(int type, int x, int y);
    void resolveCombat(int attackerId, int targetId);
    void collectResource(int unitId, int resourceId);
    void healUnit(int medicId, int targetId);
    void buildBuilding(const std::string& type, int x, int y);
    void startResearch(const std::string& tech);
    void distributeResources(const std::string& from, const std::string& to, int amount);
    void produceUnit(int buildingId, const std::string& unitType);
    void getStats(const std::string& sub);
};