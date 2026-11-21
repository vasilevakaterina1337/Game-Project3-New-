#pragma once
#include <string>
#include <iostream>

class Building {
protected:
    std::string type;
    int hp;
    int progress;
    int x, y;
public:
    Building(std::string t, int x, int y) : type(t), hp(100), progress(0), x(x), y(y) {}
    virtual ~Building() = default;
    virtual void update() { 
        if (progress < 100) 
            progress += 10; 
    }
    virtual void produce() { std::cout << "Здание " << type << " производит юнита\n"; }
    std::string getType() const { 
        return type; 
    }
    int getProgress() const { 
        return progress;
    }
    int getX() const { 
        return x; 
    }
    int getY() const { 
        return y; 
    }

};
class Barracks : public Building {
public:
    Barracks(int x, int y) : Building("barracks", x, y) {}
    void produce() override { std::cout << "Барак производит юнита\n"; }
};

class Laboratory : public Building {
public:
    Laboratory(int x, int y) : Building("lab", x, y) {}
    void produce() override { std::cout << "Лаборатория исследует технологию\n"; }
};

class Warehouse : public Building {
public:
    Warehouse(int x, int y) : Building("warehouse", x, y) {}
};

class PowerPlant : public Building {
public:
    PowerPlant(int x, int y) : Building("powerplant", x, y) {}
};

class Hospital : public Building {
public:
    Hospital(int x, int y) : Building("hospital", x, y) {}
};
