#include "Base.h"

void Base::onUnitEvent(const UnitEvent& event) {
    if (event.type == UnitEvent::Destroyed) {
        std::cout << "[База " << id << "] Уведомление: Юнит " << event.unitId << " уничтожен. Удаление из списка\n";
        units.erase(std::remove_if(units.begin(), units.end(),
            [&](const std::shared_ptr<Unit>& u) { 
                return u->getId() == event.unitId; 
            }), units.end());
    }
    else if (event.type == UnitEvent::Created) {
        std::cout << "[База " << id << "] Уведомление: Юнит " << event.unitId << " создан\n";
    }
}

void Base::addUnit(std::shared_ptr<Unit> unit) {
    if (units.size() < maxUnits) {
        units.push_back(unit);
    }
}