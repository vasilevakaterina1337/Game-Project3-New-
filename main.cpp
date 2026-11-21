#include <iostream>
#include <limits>
#include <string>
#include <locale>
#include <windows.h>
#include "GameFacade.h"
#include "Building.h"
#ifdef max
#undef max
#endif


//-642 33 3033
int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");

    auto eventBus = std::make_shared<IObservable>();
    auto base = std::make_shared<Base>(1);
    GameField field(10, 10);
    GameFacade game(field, eventBus, base);

    int choice;
    while (true) {
        std::cout << "\nМЕНЮ\n"
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
        std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
        if (choice == 0) break;

        int x, y, id, targetId, typeChoice, terrainChoice, objectChoice, amount, buildingId;
        std::string s1, s2;

        switch (choice) {
        case 1:
            std::cout << "Тип (1-Пех, 2-Луч, 3-Кав, 4-Раб, 5-Разв, 6-Танк, 7-Мед): "; std::cin >> typeChoice;
            std::cout << "X: "; std::cin >> x;
            std::cout << "Y: "; std::cin >> y;
            game.executeCommand(std::make_shared<CreateUnitCommand>(*game.getMediator(), typeChoice, x, y));
            break;
        case 2:
            std::cout << "ID: "; std::cin >> id;
            std::cout << "X: "; std::cin >> x;
            std::cout << "Y: "; std::cin >> y;
            game.executeCommand(std::make_shared<MoveUnitCommand>(*game.getMediator(), id, x, y));
            break;
        case 3:
            std::cout << "ID атакующего: "; std::cin >> id;
            std::cout << "ID цели: "; std::cin >> targetId;
            game.executeCommand(std::make_shared<AttackCommand>(*game.getMediator(), id, targetId));
            break;
        case 4:
            std::cout << "ID Рабочего: "; std::cin >> id;
            game.executeCommand(std::make_shared<CollectResourceCommand>(*game.getMediator(), id, 1));
            break;
        case 5:
            std::cout << "ID Медика: "; std::cin >> id;
            std::cout << "ID цели: "; std::cin >> targetId;
            game.executeCommand(std::make_shared<HealCommand>(*game.getMediator(), id, targetId));
            break;
        case 6: {
            std::cout << "1. Барак\n"
                << "2. Лаборатория\n"
                <<"3. Склад\n"
                <<"4. Электростанция\n"
                <<"5. Госпиталь\n"
                << "Ваш выбор : ";
            std::cin >> typeChoice;
            std::string bType;
            if (typeChoice == 1) bType = "barracks";
            else if (typeChoice == 2) bType = "lab";
            else if (typeChoice == 3) bType = "warehouse";
            else if (typeChoice == 4) bType = "powerplant";
            else if (typeChoice == 5) bType = "hospital";
            else { std::cout << "Неверно\n"; break; }
            std::cout << "X: "; std::cin >> x;
            std::cout << "Y: "; std::cin >> y;
            game.executeCommand(std::make_shared<BuildBuildingCommand>(*game.getMediator(), bType, x, y));
            break;
        }
        case 7:
            std::cout << "Технология: "; std::cin >> s1;
            game.executeCommand(std::make_shared<ResearchCommand>(*game.getMediator(), s1));
            break;
        case 8: {
            std::cout << "1. Дерево\n"
                << "2. Металл\n"
                << "3. Энергия\n"
                << "Откуда: ";
            std::cin >> typeChoice;
            if (typeChoice == 1) s1 = "wood";
            else if (typeChoice == 2) s1 = "metal";
            else if (typeChoice == 3) s1 = "energy";
            else { std::cout << "Неверно.\n"; break; }

            std::cout << "1. Барак\n"
                << "2. Лаборатория\n"
                << "3. Склад\n"
                << "Куда: ";
            std::cin >> objectChoice;
            if (objectChoice == 1) s2 = "barracks";
            else if (objectChoice == 2) s2 = "lab";
            else if (objectChoice == 3) s2 = "warehouse";
            else { std::cout << "Неверно\n"; break; }

            std::cout << "Сколько: "; std::cin >> amount;
            game.executeCommand(std::make_shared<DistributeResourcesCommand>(*game.getMediator(), s1, s2, amount));
            break;
        }
        case 9: {
            std::cout << "ID здания для производства: ";
            std::cin >> buildingId;

            std::cout << "Выберите тип юнита:\n"
                << "1. Пехотинец (infantry)\n"
                << "2. Лучник (archer)\n"
                << "3. Кавалерия (cavalry)\n"
                << "4. Рабочий (worker)\n"
                << "5. Разведчик (scout)\n"
                << "6. Танк (tank)\n"
                << "7. Медик (medic)\n"
                << "Ваш выбор: ";
            std::cin >> typeChoice;
            std::string ut;
            switch (typeChoice) {
            case 1: ut = "infantry"; break;
            case 2: ut = "archer";   break;
            case 3: ut = "cavalry";  break;
            case 4: ut = "worker";   break;
            case 5: ut = "scout";    break;
            case 6: ut = "tank";     break;
            case 7: ut = "medic";    break;
            default:
                std::cout << "Неверный тип юнита.\n";
                break;
            }
            if (!ut.empty()) {
                game.executeCommand(std::make_shared<ProduceUnitCommand>(*game.getMediator(), buildingId, ut));
            }
            break;
        }
        case 10:
            field.printMap(); break;
        case 11:
            game.printUnits(); break;
        case 12: {
            std::cout << "1. Всё\n2. Юниты\n3. Здания\n4. Ресурсы\nВыбор: ";
            std::cin >> typeChoice;
            std::string sub = "all";
            if (typeChoice == 2) sub = "units";
            else if (typeChoice == 3) sub = "buildings";
            else if (typeChoice == 4) sub = "resources";
            else if (typeChoice != 1) { std::cout << "Неверно\n"; break; }
            game.executeCommand(std::make_shared<StatsCommand>(*game.getMediator(), sub));
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
                std::cout << "Неверный тип ландшафта.\n";
                break;
            }

            std::cout << "X: "; std::cin >> x;
            std::cout << "Y: "; std::cin >> y;

            game.placeTerrain(terrainChoice, x, y);
            break;
        }
        case 14: {
            std::cout << "Выберите нейтральный объект:\n"
                << "1. Сундук\n"
                << "2. Ловушка\n"
                << "3. Алтарь\n"
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
