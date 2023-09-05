#pragma once

#include <ExLexer.hpp>
#include <EvalNode.hpp>

EvalNode* parse(const ExLexem& lexem, EvalFactory& factory);
