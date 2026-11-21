#include "GameMediator.h"
#include "Factories.h"
#include <iostream>
#include <algorithm>

GameMediator::GameMediator(GameField& f, std::shared_ptr<IObservable> bus, Base& b)
    : field(f), base(b), eventBus(bus), unitMgr(f), buildMgr(), resources(), combat() {
}

std::shared_ptr<Unit> GameMediator::findUnit(int id) const {
    return unitMgr.getUnitById(id);
}

void GameMediator::addUnit(std::shared_ptr<Unit> unit, int x, int y) {
    if (field.addUnit(unit, x, y)) {
        unitMgr.addExisting(unit);
        eventBus->notify({ UnitEvent::Created, unit->getId() });
    }
    else {
        std::cout << "Невозможно разместить юнита на (" << x << "," << y << ").\n";
        base.onUnitEvent({ UnitEvent::Destroyed, unit->getId() });
    }
}

void GameMediator::moveUnit(int unitId, int x, int y) {
    auto u = unitMgr.getUnitById(unitId);
    if (!u) { 
        std::cout << "Юнит не найден\n"; 
        return; 
    }

    u->setOrder("move to " + std::to_string(x) + "," + std::to_string(y));

    int oldX = u->getX(), oldY = u->getY();
    if (field.getUnitAt(oldX, oldY) == u) field.removeUnit(oldX, oldY);
    if (field.addUnit(u, x, y)) {
        std::cout << "[Медиатор] Юнит " << unitId << " перемещен\n";
    }
    else {
        field.addUnit(u, oldX, oldY);
        std::cout << "Невозможно переместить юнита\n";
    }
}

void GameMediator::attackUnit(int attackerId, int targetId) {
    resolveCombat(attackerId, targetId);
}

void GameMediator::interactWithObject(int unitId, int x, int y) {
    auto obj = field.getObject(x, y);
    auto u = unitMgr.getUnitById(unitId);
    if (!obj || !u) { 
        std::cout << "Объект или юнит не найдены.\n"; 
        return; 
    }
    *obj + *u;
}

void GameMediator::setTerrain(int x, int y, std::shared_ptr<Terrain> t) {
    field.setTerrain(x, y, t);
}

void GameMediator::setObject(int x, int y, std::shared_ptr<NeutralObject> o) {
    field.setObject(x, y, o);
}

void GameMediator::setBuilding(int x, int y, std::shared_ptr<Building> b) {
    field.addBuilding(b, x, y);
    buildMgr.add(b);
}

void GameMediator::createUnit(int type, int x, int y) {
    std::unique_ptr<UnitFactory> f;
    switch (type) {
    case 1: f = std::make_unique<InfantryFactory>(); 
        break;
    case 2: f = std::make_unique<ArcherFactory>();   
        break;
    case 3: f = std::make_unique<CavalryFactory>();  
        break;
    case 4: f = std::make_unique<WorkerFactory>();   
        break;
    case 5: f = std::make_unique<ScoutFactory>();   
        break;
    case 6: f = std::make_unique<TankFactory>();     
        break;
    case 7: f = std::make_unique<MedicFactory>();   
        break;
    default: std::cout << "Неверный тип юнита\n";
        return;
    }
    int id = unitMgr.allocateId();
    auto u = std::shared_ptr<Unit>(f->createUnit(id).release());
    if (field.addUnit(u, x, y)) {
        base.addUnit(u);
        unitMgr.addExisting(u);
        eventBus->notify({ UnitEvent::Created, u->getId() });
        std::cout << "[Медиатор] Создан " << u->getName() << " id=" << id << "\n";
    }
    else {
        std::cout << "Клетка занята\n";
    }
}

void GameMediator::resolveCombat(int attackerId, int targetId) {
    auto atk = unitMgr.getUnitById(attackerId);
    auto tgt = unitMgr.getUnitById(targetId);
    if (!atk || !tgt) { 
        std::cout << "Цель или атакующий не найдены\n"; 
        return; 
    }

    atk->setOrder("attack unit " + std::to_string(targetId));

    int dmg = combat.calculateDamage(*atk, *tgt);
    tgt->takeDamage(dmg);
    eventBus->notify({ UnitEvent::Damaged, targetId });
    std::cout << "[Медиатор] " << atk->getName() << " нанёс " << dmg << " урона\n";

    if (!tgt->isAlive()) {
        if (field.getUnitAt(tgt->getX(), tgt->getY()) == tgt) field.removeUnit(tgt->getX(), tgt->getY());
        eventBus->notify({ UnitEvent::Destroyed, targetId });
        unitMgr.removeUnit(targetId);
    }
}

void GameMediator::collectResource(int unitId, int) {
    auto u = unitMgr.getUnitById(unitId);
    if (!u || u->getType() != UnitType::Worker) { std::cout << "Нужен Рабочий\n"; return; }

    u->setOrder("collect");

    resources.addWood(50);
    std::cout << "[Медиатор] Собрано 50 дерева (всего: " << resources.wood << ")\n";
}

void GameMediator::healUnit(int medicId, int targetId) {
    auto m = unitMgr.getUnitById(medicId);
    auto t = unitMgr.getUnitById(targetId);
    if (!m || m->getType() != UnitType::Medic) { 
        std::cout << "Нужен Медик\n"; 
        return; 
    }
    if (!t) return;

    m->setOrder("heal unit " + std::to_string(targetId));

    t->heal(30);
    std::cout << "[Медиатор] Вылечено 30 HP\n";
}

void GameMediator::buildBuilding(const std::string& type, int x, int y) {
    if (!resources.spendWood(100)) return;

    std::shared_ptr<Building> b;
    if (type == "barracks")        b = std::make_shared<Barracks>(x, y);
    else if (type == "lab")        b = std::make_shared<Laboratory>(x, y);
    else if (type == "warehouse")  b = std::make_shared<Warehouse>(x, y);
    else if (type == "powerplant") b = std::make_shared<PowerPlant>(x, y);
    else if (type == "hospital")   b = std::make_shared<Hospital>(x, y);
    else {
        std::cout << "[Ошибка] Неизвестный тип здания: " << type << "\n";
        return;
    }

    base.addBuilding(b);
    setBuilding(x, y, b);
    std::cout << "[Медиатор] Построено " << type << " на (" << x << "," << y << ")\n";
}

void GameMediator::startResearch(const std::string& tech) {
    researchName = tech; researchRemaining = 3;
    std::cout << "[Медиатор] Исследование " << tech << " запущено\n";
}

void GameMediator::distributeResources(const std::string& from, const std::string& to, int amount) {
    resources.distribute(from, to, amount);
}

void GameMediator::produceUnit(int buildingId, const std::string& unitType) {
    auto b = buildMgr.getById(buildingId);
    if (!b) { std::cout << "Здание не найдено\n"; return; }
    b->produce();
    int type = 1;
    if (unitType == "infantry") type = 1;
    else if (unitType == "archer") type = 2;
    else if (unitType == "cavalry") type = 3;
    else if (unitType == "worker") type = 4;
    else if (unitType == "scout") type = 5;
    else if (unitType == "tank") type = 6;
    else if (unitType == "medic") type = 7;
    createUnit(type, b->getX(), b->getY());
}

void GameMediator::getStats(const std::string& sub) {
    if (sub.empty() || sub == "all") {
        std::cout << "Ресурсы: wood=" << resources.wood << " metal=" << resources.metal << " energy=" << resources.energy << "\n";
        std::cout << "Юниты (" << unitMgr.all().size() << "):\n";
        for (auto& u : unitMgr.all()) {
            std::cout << "  #" << u->getId() << " " << u->getName()
                << " HP=" << u->getHealth()
                << " pos=(" << u->getX() << "," << u->getY() << ")"
                << " order=" << u->getOrder() << "\n";
        }
        std::cout << "Здания (" << buildMgr.all().size() << "):\n";
        for (auto& b : buildMgr.all()) {
            std::cout << "  " << b->getType() << " pos=(" << b->getX() << "," << b->getY() << ")\n";
        }
    }
    else if (sub == "units") {
        for (auto& u : unitMgr.all()) {
            std::cout << "  #" << u->getId() << " " << u->getName()
                << " HP=" << u->getHealth()
                << " order=" << u->getOrder() << "\n";
        }
    }
    else if (sub == "buildings") {
        for (auto& b : buildMgr.all()) {
            std::cout << "  " << b->getType() << "\n";
        }
    }
    else if (sub == "resources") {
        std::cout << "wood=" << resources.wood << " metal=" << resources.metal << " energy=" << resources.energy << "\n";
    }
}
