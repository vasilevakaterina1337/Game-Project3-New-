#pragma once
#include <vector>
#include <memory>
#include <iostream>
#include <typeinfo>
#include "Unit.h"
#include "Terrain.h"
#include "NeutralObjects.h"
#include "Building.h"

class GameField {
    int width, height;
    std::vector<std::vector<std::shared_ptr<Unit>>> grid;
    std::vector<std::vector<std::shared_ptr<Terrain>>> terrains;
    std::vector<std::vector<std::shared_ptr<NeutralObject>>> objects;
    std::vector<std::vector<std::shared_ptr<Building>>> buildingsGrid;

public:
    GameField(int w, int h);
    int getWidth() const {
        return width; 
    }
    int getHeight() const { 
        return height; 
    }

    class Iterator {
        GameField& field;
        int curX = 0, curY = 0;
    public:
        Iterator(GameField& f);
        bool operator!=(const Iterator& other) const { 
            return curX != other.curX || curY != other.curY; 
        }
        Iterator& operator++();
        std::shared_ptr<Unit> operator*() const { 
            return field.grid[curY][curX]; 
        }
        void skipEmpty();
    };

    Iterator begin() { 
        return Iterator(*this); 
    }
    Iterator end() { 
        return Iterator(*this); 
    }
    bool addUnit(std::shared_ptr<Unit> unit, int x, int y);
    void removeUnit(int x, int y);
    void setTerrain(int x, int y, std::shared_ptr<Terrain> t);
    void setObject(int x, int y, std::shared_ptr<NeutralObject> obj);
    std::shared_ptr<NeutralObject> getObject(int x, int y);
    std::shared_ptr<Unit> getUnitAt(int x, int y);

    void addBuilding(std::shared_ptr<Building> b, int x, int y);

    void printMap() const;
};

inline GameField::GameField(int w, int h) : width(w), height(h) {
    grid.resize(height, std::vector<std::shared_ptr<Unit>>(width, nullptr));
    terrains.resize(height, std::vector<std::shared_ptr<Terrain>>(width, std::make_shared<Plains>()));
    objects.resize(height, std::vector<std::shared_ptr<NeutralObject>>(width, nullptr));

    buildingsGrid.resize(height, std::vector<std::shared_ptr<Building>>(width, nullptr));
}

inline GameField::Iterator::Iterator(GameField& f) : field(f) { 
    skipEmpty(); 
}

inline GameField::Iterator& GameField::Iterator::operator++() {
    curX++;
    if (curX >= field.width) { 
        curX = 0; curY++; 
    }
    skipEmpty();
    return *this;
}

inline void GameField::Iterator::skipEmpty() {
    while (curY < field.height && !field.grid[curY][curX]) {
        curX++;
        if (curX >= field.width) { 
            curX = 0; curY++; 
        }
    }
}

inline bool GameField::addUnit(std::shared_ptr<Unit> unit, int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height && !grid[y][x]) {
        if (terrains[y][x]->isPassable(*unit)) {
            grid[y][x] = unit;
            unit->setPosition(x, y);
            return true;
        }
    }
    return false;
}

inline void GameField::removeUnit(int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height) grid[y][x] = nullptr;
}

inline void GameField::setTerrain(int x, int y, std::shared_ptr<Terrain> t) {
    if (x >= 0 && x < width && y >= 0 && y < height) 
        terrains[y][x] = std::make_shared<TerrainProxy>(t);
}

inline void GameField::setObject(int x, int y, std::shared_ptr<NeutralObject> obj) {
    if (x >= 0 && x < width && y >= 0 && y < height) 
        objects[y][x] = obj;
}

inline std::shared_ptr<NeutralObject> GameField::getObject(int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height) 
        return objects[y][x];
    return nullptr;
}

inline std::shared_ptr<Unit> GameField::getUnitAt(int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height) 
        return grid[y][x];
    return nullptr;
}

inline void GameField::addBuilding(std::shared_ptr<Building> b, int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        buildingsGrid[y][x] = b;
    }
}

inline void GameField::printMap() const {
    std::cout << "\n   ";
    for (int i = 0; i < width; ++i) std::cout << i << " ";
    std::cout << "\n";

    for (int y = 0; y < height; ++y) {
        std::cout << y << "  ";
        for (int x = 0; x < width; ++x) {
            char c = terrains[y][x]->getSymbol();

            if (buildingsGrid[y][x]) {
                c = 'B';
            }

            if (objects[y][x]) {
                c = 'O';
            }
            if (grid[y][x]) {
                switch (grid[y][x]->getType()) {
                case UnitType::Infantry: c = 'I'; 
                    break;
                case UnitType::Archer: c = 'A'; 
                    break;
                case UnitType::Cavalry: c = 'C'; 
                    break;
                case UnitType::Worker:c = 'W'; 
                    break;
                case UnitType::Scout:c = 'S'; 
                    break;
                case UnitType::Tank: c = 'T'; 
                    break;
                case UnitType::Medic: c = 'M'; 
                    break;
                }
            }

            std::cout << c << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\nЛегенда: . - Равнина, ^ - Горы, * - Лес, ~ - Болото, O - Объект, B - Здание\n"
    << "         I - Пехотинец, A - Лучник, C - Кавалерия, W - Рабочий\n"
    << "         S - Разведчик, T - Танк, M - Медик\n";
}