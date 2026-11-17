#include "Handlers.h"
#include <sstream>
#include <iostream>

namespace {
    bool parseComma2(const std::string& s, int& x, int& y) {
        size_t c = s.find(',');
        if (c == std::string::npos) return false;
        try {
            x = std::stoi(s.substr(0, c));
            y = std::stoi(s.substr(c + 1));
            return true;
        }
        catch (...) {
            return false;
        }
    }
}

bool CreateHandler::handle(const std::string& cmd, CommandHistory& hist) {
    std::istringstream ss(cmd);
    std::string a, b, type, at, pos;
    if (ss >> a >> b >> type >> at >> pos && a == "create" && b == "unit" && at == "at") {
        int x, y;
        if (!parseComma2(pos, x, y)) {
            std::cout << "[Парсер] Неверные координаты: " << pos << "\n";
            return true;
        }
        int t = 0;
        if (type == "infantry") t = 1;
        else if (type == "archer")   t = 2;
        else if (type == "cavalry")  t = 3;
        else if (type == "worker")   t = 4;
        else if (type == "scout")    t = 5;
        else if (type == "tank")     t = 6;
        else if (type == "medic")    t = 7;
        else {
            std::cout << "[Парсер] Неизвестный тип юнита: " << type << "\n";
            return true;
        }
        auto c = std::make_shared<CreateUnitCommand>(mediator, t, x, y);
        hist.push(c);
        c->execute();
        return true;
    }
    return next ? next->handle(cmd, hist) : false;
}

bool MoveHandler::handle(const std::string& cmd, CommandHistory& hist) {
    std::istringstream ss(cmd);
    std::string a, b, idStr, to, pos;
    if (ss >> a >> b >> idStr >> to >> pos && a == "move" && b == "unit" && to == "to") {
        int x, y;
        if (!parseComma2(pos, x, y)) {
            std::cout << "[Парсер] Неверные координаты: " << pos << "\n";
            return true;
        }
        int id = 0;
        try { id = std::stoi(idStr); }
        catch (...) {
            std::cout << "[Парсер] Неверный id: " << idStr << "\n";
            return true;
        }
        auto c = std::make_shared<MoveUnitCommand>(mediator, id, x, y);
        hist.push(c);
        c->execute();
        return true;
    }
    return next ? next->handle(cmd, hist) : false;
}

bool AttackHandler::handle(const std::string& cmd, CommandHistory& hist) {
    std::istringstream ss(cmd);
    std::string a, b, idA, kw, idT;
    if (ss >> a >> b >> idA >> kw >> idT
        && a == "attack" && b == "unit" && kw == "target") {
        try {
            auto c = std::make_shared<AttackCommand>(mediator,
                std::stoi(idA), std::stoi(idT));
            hist.push(c);
            c->execute();
        }
        catch (...) {
            std::cout << "[Парсер] Неверные id в attack\n";
        }
        return true;
    }
    return next ? next->handle(cmd, hist) : false;
}

bool CollectHandler::handle(const std::string& cmd, CommandHistory& hist) {
    std::istringstream ss(cmd);
    std::string a, b, idS, from, resS;
    if (ss >> a >> b >> idS >> from >> resS
        && a == "collect" && b == "unit" && from == "from") {
        try {
            auto c = std::make_shared<CollectResourceCommand>(mediator,
                std::stoi(idS), std::stoi(resS));
            hist.push(c);
            c->execute();
        }
        catch (...) {
            std::cout << "[Парсер] Неверные id в collect\n";
        }
        return true;
    }
    return next ? next->handle(cmd, hist) : false;
}

bool HealHandler::handle(const std::string& cmd, CommandHistory& hist) {
    std::istringstream ss(cmd);
    std::string a, b, idS, kw, tidS;
    if (ss >> a >> b >> idS >> kw >> tidS
        && a == "heal" && b == "unit" && kw == "target") {
        try {
            auto c = std::make_shared<HealCommand>(mediator,
                std::stoi(idS), std::stoi(tidS));
            hist.push(c);
            c->execute();
        }
        catch (...) {
            std::cout << "[Парсер] Неверные id в heal\n";
        }
        return true;
    }
    return next ? next->handle(cmd, hist) : false;
}

bool BuildHandler::handle(const std::string& cmd, CommandHistory& hist) {
    std::istringstream ss(cmd);
    std::string a, type, at, pos;
    if (ss >> a >> type >> at >> pos && a == "build" && at == "at") {
        int x, y;
        if (!parseComma2(pos, x, y)) {
            std::cout << "[Парсер] Неверные координаты: " << pos << "\n";
            return true;
        }
        auto c = std::make_shared<BuildBuildingCommand>(mediator, type, x, y);
        hist.push(c);
        c->execute();
        return true;
    }
    return next ? next->handle(cmd, hist) : false;
}

bool ResearchHandler::handle(const std::string& cmd, CommandHistory& hist) {
    std::istringstream ss(cmd);
    std::string a, tech;
    if (ss >> a >> tech && a == "research") {
        auto c = std::make_shared<ResearchCommand>(mediator, tech);
        hist.push(c);
        c->execute();
        return true;
    }
    return next ? next->handle(cmd, hist) : false;
}

bool ProduceHandler::handle(const std::string& cmd, CommandHistory& hist) {
    std::istringstream ss(cmd);
    std::string a, ut, in, bidS;
    if (ss >> a >> ut >> in >> bidS && a == "produce" && in == "in") {
        try {
            auto c = std::make_shared<ProduceUnitCommand>(mediator,
                std::stoi(bidS), ut);
            hist.push(c);
            c->execute();
        }
        catch (...) {
            std::cout << "[Парсер] Неверный buildingId в produce\n";
        }
        return true;
    }
    return next ? next->handle(cmd, hist) : false;
}

bool DistributeHandler::handle(const std::string& cmd, CommandHistory& hist) {
    std::istringstream ss(cmd);
    std::string a, from, to, target; int amount;
    if (ss >> a >> from >> to >> target >> amount
        && a == "distribute" && to == "to") {
        auto c = std::make_shared<DistributeResourcesCommand>(mediator,
            from, target, amount);
        hist.push(c);
        c->execute();
        return true;
    }
    return next ? next->handle(cmd, hist) : false;
}

bool StatsHandler::handle(const std::string& cmd, CommandHistory& hist) {
    std::istringstream ss(cmd);
    std::string a;
    if (ss >> a && a == "stats") {
        std::string sub;
        ss >> sub;
        auto c = std::make_shared<StatsCommand>(mediator, sub);
        hist.push(c);
        c->execute();
        return true;
    }
    return next ? next->handle(cmd, hist) : false;
}