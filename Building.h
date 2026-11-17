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
