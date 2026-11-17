#pragma once
#include <vector>
#include <memory>
#include "Commands.h"

class CommandHistory {
    std::vector<std::shared_ptr<Command>> history;
public:
    void push(std::shared_ptr<Command> cmd) {
        history.push_back(std::move(cmd));
    }
    void undo() {
        if (!history.empty()) history.pop_back();
    }
    std::shared_ptr<Command> getLastCommand() const {
        return history.empty() ? nullptr : history.back();
    }
    size_t size() const { return history.size(); }
};