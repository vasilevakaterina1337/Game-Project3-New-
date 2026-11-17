#pragma once
#include <string>
#include "GameMediator.h"

class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
};

class CreateUnitCommand : public Command {
    GameMediator& mediator;
    int type, x, y;
public:
    CreateUnitCommand(GameMediator& m, int type, int x, int y)
        : mediator(m), type(type), x(x), y(y) {
    }
    void execute() override { mediator.createUnit(type, x, y); }
};

class MoveUnitCommand : public Command {
    GameMediator& mediator;
    int unitId, x, y;
public:
    MoveUnitCommand(GameMediator& m, int id, int nx, int ny)
        : mediator(m), unitId(id), x(nx), y(ny) {
    }
    void execute() override { mediator.moveUnit(unitId, x, y); }
};

class AttackCommand : public Command {
    GameMediator& mediator;
    int attackerId, targetId;
public:
    AttackCommand(GameMediator& m, int atk, int tgt)
        : mediator(m), attackerId(atk), targetId(tgt) {
    }
    void execute() override { mediator.resolveCombat(attackerId, targetId); }
};

class CollectResourceCommand : public Command {
    GameMediator& mediator;
    int unitId, resourceId;
public:
    CollectResourceCommand(GameMediator& m, int u, int r)
        : mediator(m), unitId(u), resourceId(r) {
    }
    void execute() override { mediator.collectResource(unitId, resourceId); }
};

class HealCommand : public Command {
    GameMediator& mediator;
    int medicId, targetId;
public:
    HealCommand(GameMediator& m, int mid, int tid)
        : mediator(m), medicId(mid), targetId(tid) {
    }
    void execute() override { mediator.healUnit(medicId, targetId); }
};

class BuildBuildingCommand : public Command {
    GameMediator& mediator;
    std::string type;
    int x, y;
public:
    BuildBuildingCommand(GameMediator& m, std::string t, int x, int y)
        : mediator(m), type(std::move(t)), x(x), y(y) {
    }
    void execute() override { mediator.buildBuilding(type, x, y); }
};

class ResearchCommand : public Command {
    GameMediator& mediator;
    std::string tech;
public:
    ResearchCommand(GameMediator& m, std::string t)
        : mediator(m), tech(std::move(t)) {
    }
    void execute() override { mediator.startResearch(tech); }
};

class DistributeResourcesCommand : public Command {
    GameMediator& mediator;
    std::string from, to;
    int amount;
public:
    DistributeResourcesCommand(GameMediator& m, std::string f, std::string t, int a)
        : mediator(m), from(std::move(f)), to(std::move(t)), amount(a) {
    }
    void execute() override { mediator.distributeResources(from, to, amount); }
};

class ProduceUnitCommand : public Command {
    GameMediator& mediator;
    int buildingId;
    std::string unitType;
public:
    ProduceUnitCommand(GameMediator& m, int bid, std::string ut)
        : mediator(m), buildingId(bid), unitType(std::move(ut)) {
    }
    void execute() override { mediator.produceUnit(buildingId, unitType); }
};

class StatsCommand : public Command {
    GameMediator& mediator;
    std::string sub;
public:
    StatsCommand(GameMediator& m, std::string s = "")
        : mediator(m), sub(std::move(s)) {
    }
    void execute() override { mediator.getStats(sub); }
};