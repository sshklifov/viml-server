#pragma once

#include <ExLexer.hpp>
#include <EvalFactory.hpp>

EvalCommand* parse(const ExLexem& lexem, EvalFactory& factory);
