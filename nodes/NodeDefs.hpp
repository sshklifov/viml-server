#pragma once

#include "BaseNode.hpp"

#include "LogicNodes.hpp"
#include "LoopNodes.hpp"
#include "FunctionNodes.hpp"
#include "ExceptionNodes.hpp"

struct RootNode : GroupNode {
    RootNode() : GroupNode(ExLexem()) {}

    int getId() override {
        assert(false);
        return CMD_SIZE;
    }

    void parse(DiagnosticReporter& rep) override { assert(false); }
};

struct ExNode : BaseNode {
    ExNode(const ExLexem& lexem) : BaseNode(lexem) {}
};
