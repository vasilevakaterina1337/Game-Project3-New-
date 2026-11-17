#include "GameFacade.h"
#include "Building.h"
#include <iostream>

GameFacade::GameFacade(GameField& field, std::shared_ptr<IObservable> bus, std::shared_ptr<Base> b)
    : base(b) {
    mediator = std::make_shared<GameMediator>(field, bus, *b);
    bus->subscribe(b.get());

    auto createH = std::make_shared<CreateHandler>(*mediator);
    auto moveH = std::make_shared<MoveHandler>(*mediator);
    auto atkH = std::make_shared<AttackHandler>(*mediator);
    auto collectH = std::make_shared<CollectHandler>(*mediator);
    auto healH = std::make_shared<HealHandler>(*mediator);
    auto buildH = std::make_shared<BuildHandler>(*mediator);
    auto researchH = std::make_shared<ResearchHandler>(*mediator);
    auto produceH = std::make_shared<ProduceHandler>(*mediator);
    auto distributeH = std::make_shared<DistributeHandler>(*mediator);
    auto statsH = std::make_shared<StatsHandler>(*mediator);

    createH->setNext(moveH);
    moveH->setNext(atkH);
    atkH->setNext(collectH);
    collectH->setNext(healH);
    healH->setNext(buildH);
    buildH->setNext(researchH);
    researchH->setNext(produceH);
    produceH->setNext(distributeH);
    distributeH->setNext(statsH);
    chain = createH;
}

void GameFacade::processInput(const std::string& input) {
    if (!chain->handle(input, history))
        std::cout << "[Фасад] Неизвестная команда: " << input << "\n";
}

void GameFacade::placeTerrain(int terrainChoice, int x, int y) {
    std::shared_ptr<Terrain> t;
    switch (terrainChoice) {
    case 1: t = std::make_shared<Plains>(); 
        break;
    case 2: t = std::make_shared<Forest>(); 
        break;
    case 3: t = std::make_shared<Swamp>(); 
        break;
    case 4: t = std::make_shared<Mountains>(); 
        break;
    default: return;
    }
    mediator->setTerrain(x, y, t);
}

void GameFacade::placeObject(int objectChoice, int x, int y) {
    std::shared_ptr<NeutralObject> o;
    switch (objectChoice) {
    case 1: o = std::make_shared<TreasureChest>(); 
        break;
    case 2: o = std::make_shared<Trap>(); 
        break;
    case 3: o = std::make_shared<Altar>(); 
        break;
    default: return;
    }
    mediator->setObject(x, y, o);
}

void GameFacade::printUnits() {
    std::cout << "Юниты (" << base->getUnitCount() << "):\n";
    for (auto& u : base->getUnits()) {
        std::cout << "  ID: " << u->getId() << " | " << u->getName()
            << " | HP: " << u->getHealth() << " | (" << u->getX() << "," << u->getY() << ")\n";
    }
}