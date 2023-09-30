#pragma once

#include "BaseNode.hpp"

#include "LogicNodes.hpp"
#include "LoopNodes.hpp"
#include "FunctionNodes.hpp"
#include "ExceptionNodes.hpp"
#include "EvalNodes.hpp"
#include "LetNodes.hpp"
#include "EchoNodes.hpp"

struct RootNode : GroupNode {
    RootNode() : GroupNode(ExLexem()) {}

    int getId() override { return CMD_SIZE; }

    void parse(DiagnosticReporter& rep, const char*& nextcmd) override { assert(false); }
};

template <typename T>
void* defaultNodeCreator(void* user) {
    return new T(*(ExLexem*)user);
}
