#pragma once
#include <string>
#include <memory>
#include "Commands.h"
#include "CommandHistory.h"

class Handler {
protected:
    std::shared_ptr<Handler> next;
public:
    void setNext(std::shared_ptr<Handler> n) { next = std::move(n); }
    virtual bool handle(const std::string& cmd, CommandHistory& hist) = 0;
    virtual ~Handler() = default;
};

class CreateHandler : public Handler {
    GameMediator& mediator;
public:
    CreateHandler(GameMediator& m) : mediator(m) {}
    bool handle(const std::string& cmd, CommandHistory& hist) override;
};

class MoveHandler : public Handler {
    GameMediator& mediator;
public:
    MoveHandler(GameMediator& m) : mediator(m) {}
    bool handle(const std::string& cmd, CommandHistory& hist) override;
};

class AttackHandler : public Handler {
    GameMediator& mediator;
public:
    AttackHandler(GameMediator& m) : mediator(m) {}
    bool handle(const std::string& cmd, CommandHistory& hist) override;
};

class CollectHandler : public Handler {
    GameMediator& mediator;
public:
    CollectHandler(GameMediator& m) : mediator(m) {}
    bool handle(const std::string& cmd, CommandHistory& hist) override;
};

class HealHandler : public Handler {
    GameMediator& mediator;
public:
    HealHandler(GameMediator& m) : mediator(m) {}
    bool handle(const std::string& cmd, CommandHistory& hist) override;
};

class BuildHandler : public Handler {
    GameMediator& mediator;
public:
    BuildHandler(GameMediator& m) : mediator(m) {}
    bool handle(const std::string& cmd, CommandHistory& hist) override;
};

class ResearchHandler : public Handler {
    GameMediator& mediator;
public:
    ResearchHandler(GameMediator& m) : mediator(m) {}
    bool handle(const std::string& cmd, CommandHistory& hist) override;
};

class ProduceHandler : public Handler {
    GameMediator& mediator;
public:
    ProduceHandler(GameMediator& m) : mediator(m) {}
    bool handle(const std::string& cmd, CommandHistory& hist) override;
};

class DistributeHandler : public Handler {
    GameMediator& mediator;
public:
    DistributeHandler(GameMediator& m) : mediator(m) {}
    bool handle(const std::string& cmd, CommandHistory& hist) override;
};

class StatsHandler : public Handler {
    GameMediator& mediator;
public:
    StatsHandler(GameMediator& m) : mediator(m) {}
    bool handle(const std::string& cmd, CommandHistory& hist) override;
};