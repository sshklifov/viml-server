#pragma once

#include <ExLexer.hpp>
#include <EvalCommand.hpp>

EvalCommand* parse(const ExLexem& lexem, EvalFactory& factory);
