#include <iostream>
#include <limits>
#include <string>
#include <locale>
#include "GameFacade.h"
#include "Building.h"
//-642 33 3033
int main() {

    setlocale(LC_ALL, "Russian");

    auto eventBus = std::make_shared<IObservable>();
    auto base = std::make_shared<Base>(1);
    GameField field(10, 10);
    GameFacade game(field, eventBus, base);

    int choice;
    while (true) {
        std::cout << "\nМеню\n"
            << "1. Создать юнита\n"
            << "2. Переместить\n"
            << "3. Атаковать\n"
            << "4. Собрать ресурс\n"
            << "5. Лечить\n"
            << "6. Построить здание\n"
            << "7. Исследовать\n"
            << "8. Распределить ресурсы\n"
            << "9. Произвести в здании\n"
            << "10. Показать карту\n"
            << "11. Показать юнитов\n"
            << "12. Показать статистику\n"
            << "13. Установить ландшафт\n"
            << "14. Установить объект\n"
            << "15. Взаимодействовать с объектом\n"
            << "16. Ввести текстовую команду\n"
            << "0. Выход\n"
            << "Выбор: ";
        if (!(std::cin >> choice)) break;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (choice == 0) break;

        int x, y, id, targetId, typeChoice, terrainChoice, objectChoice, amount, buildingId;
        std::string s1, s2;

        switch (choice) {
        case 1:
            std::cout << "Тип (1-Пех, 2-Луч, 3-Кав, 4-Раб, 5-Разв, 6-Танк, 7-Мед): "; std::cin >> typeChoice;
            std::cout << "X: "; std::cin >> x;
            std::cout << "Y: "; std::cin >> y;
            game.createUnit(typeChoice, x, y); break;
        case 2:
            std::cout << "ID: "; std::cin >> id;
            std::cout << "X: "; std::cin >> x;
            std::cout << "Y: "; std::cin >> y;
            game.moveUnit(id, x, y); break;
        case 3:
            std::cout << "ID атакующего: "; std::cin >> id;
            std::cout << "ID цели: "; std::cin >> targetId;
            game.attack(id, targetId); break;
        case 4:
            std::cout << "ID Рабочего: "; std::cin >> id;
            game.collectResource(id, 1); break;
        case 5:
            std::cout << "ID Медика: "; std::cin >> id;
            std::cout << "ID цели: "; std::cin >> targetId;
            game.heal(id, targetId); break;
        case 6: {
            std::cout << "Выберите тип здания:\n"
            << "1. Барак (barracks)\n"
            << "2. Лаборатория (lab)\n"
            << "3. Склад (warehouse)\n"
            << "4. Электростанция (powerplant)\n"
            << "5. Госпиталь (hospital)\n"
            << "Ваш выбор: ";
            std::cin >> typeChoice;

            std::string buildingType;
            switch (typeChoice) {
            case 1: buildingType = "barracks";
                break;
            case 2: buildingType = "lab"; 
                break;
            case 3: buildingType = "warehouse";
                break;
            case 4: buildingType = "powerplant";
                break;
            case 5: buildingType = "hospital";
                break;
            default:
                std::cout << "Неверный тип здания\n";
                break;
            }

            if (!buildingType.empty()) {
                std::cout << "X: "; std::cin >> x;
                std::cout << "Y: "; std::cin >> y;
                game.buildBuilding(buildingType, x, y);
            }
            break;
        }
        case 7:
            std::cout << "Технология: "; std::cin >> s1;
            game.research(s1); break;
        case 8: {
            std::cout << "Откуда перераспределить?\n"
            << "1. Дерево (wood)\n"
            << "2. Металл (metal)\n"
            << "3. Энергия (energy)\n"
            << "Ваш выбор: ";
            std::cin >> typeChoice;

            if (typeChoice == 1) s1 = "wood";
            else if (typeChoice == 2) s1 = "metal";
            else if (typeChoice == 3) s1 = "energy";
            else {
                std::cout << "Неверный выбор ресурса\n";
                break;
            }

            std::cout << "Куда направить?\n"
            << "1. Барак (barracks)\n"
            << "2. Лаборатория (lab)\n"
            << "3. Склад (warehouse)\n"
            << "Ваш выбор: ";
            std::cin >> objectChoice; 

            if (objectChoice == 1) s2 = "barracks";
            else if (objectChoice == 2) s2 = "lab";
            else if (objectChoice == 3) s2 = "warehouse";
            else {
                std::cout << "Неверный выбор здания.\n";
                break;
            }

            std::cout << "Сколько?: ";
            std::cin >> amount;

            game.distributeResources(s1, s2, amount);
            break;
        }
        case 9:
            std::cout << "ID здания: "; std::cin >> buildingId;
            std::cout << "Тип юнита: "; std::cin >> s1;
            game.produceUnit(buildingId, s1); 
            break;
        case 10:
            field.printMap();
            break;
        case 11:
            game.printUnits(); 
            break;
        case 12: {
            std::cout << "Что вывести?\n"
            << "1. Всё (all)\n"
            << "2. Только юнитов (units)\n"
            << "3. Только здания (buildings)\n"
            << "4. Только ресурсы (resources)\n"
            << "Ваш выбор: ";
            std::cin >> typeChoice;

            std::string sub;
            switch (typeChoice) {
            case 1: sub = "all"; 
                break;
            case 2: sub = "units";
                break;
            case 3: sub = "buildings"; 
                break;
            case 4: sub = "resources";
                break;
            default:
                std::cout << "Неверный выбор\n";
                break;
            }

            if (!sub.empty()) {
                game.getStats(sub);
            }
            break;
        }
        case 13: {
            std::cout << "Выберите тип ландшафта:\n"
            << "1. Равнина\n"
            << "2. Лес\n"
            << "3. Болото\n"
            << "4. Горы\n"
            << "Ваш выбор: ";
            std::cin >> terrainChoice;

            if (terrainChoice < 1 || terrainChoice > 4) {
                std::cout << "Неверный тип ландшафта\n";
                break;
            }

            std::cout << "X: "; std::cin >> x;
            std::cout << "Y: "; std::cin >> y;

            game.placeTerrain(terrainChoice, x, y);
            break;
        }
        case 14: {
            std::cout << "Выберите нейтральный объект:\n"
            << "1. Сундук (TreasureChest)\n"
            << "2. Ловушка (Trap)\n"
            << "3. Алтарь (Altar)\n"
            << "Ваш выбор: ";
            std::cin >> objectChoice;

            if (objectChoice < 1 || objectChoice > 3) {
                std::cout << "Неверный выбор объекта\n";
                break;
            }

            std::cout << "X: "; std::cin >> x;
            std::cout << "Y: "; std::cin >> y;

            game.placeObject(objectChoice, x, y);
            break;
        }
        case 15:
            std::cout << "ID юнита: "; std::cin >> id;
            std::cout << "X: "; std::cin >> x;
            std::cout << "Y: "; std::cin >> y;
            game.interact(id, x, y); break;
        case 16: {
            std::cout << "Команда: ";
            std::string line;
            std::getline(std::cin, line);
            if (!line.empty()) game.processInput(line);
            break;
        }
        default:
            std::cout << "Неверный ввод\n";
        }
    }
    return 0;
}