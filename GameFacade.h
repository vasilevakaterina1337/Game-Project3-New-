#pragma once
#include <memory>
#include <string>
#include "GameMediator.h"
#include "CommandHistory.h"
#include "Handlers.h"

class GameFacade {
    std::shared_ptr<GameMediator> mediator;
    std::shared_ptr<Handler> chain;
    CommandHistory history;
    std::shared_ptr<Base> base; // <--- ÄÎÁÀÂËÅÍÎ, ÷òîáû íå áûëî îøèáêè E0292

public:
    GameFacade(GameField& field, std::shared_ptr<IObservable> bus, std::shared_ptr<Base> b);

    void executeCommand(std::shared_ptr<Command> cmd) {
        history.push(cmd);
        cmd->execute();
    }

    void processInput(const std::string& input);
    void placeTerrain(int terrainChoice, int x, int y);
    void placeObject(int objectChoice, int x, int y);
    void printUnits();
    void createUnit(int type, int x, int y) {
        mediator->createUnit(type, x, y);
    }
    void moveUnit(int id, int x, int y) { 
        mediator->moveUnit(id, x, y); 
    }
    void attack(int a, int t) { 
        mediator->resolveCombat(a, t);
    }
    void collectResource(int u, int r) {
        mediator->collectResource(u, r);
    }
    void heal(int m, int t) { 
        mediator->healUnit(m, t);
    }
    void buildBuilding(const std::string& t, int x, int y) { 
        mediator->buildBuilding(t, x, y);
    }
    void research(const std::string& t) {
        mediator->startResearch(t);
    }
    void distributeResources(const std::string& f, const std::string& t, int a) {
        mediator->distributeResources(f, t, a); 
    }
    void produceUnit(int bid, const std::string& ut) {
        mediator->produceUnit(bid, ut); }
    void getStats(const std::string& s = "") { mediator->getStats(s);
    }
    void interact(int id, int x, int y) { 
        mediator->interactWithObject(id, x, y);
    }

    std::shared_ptr<GameMediator> getMediator() { 
        return mediator;
    }
};