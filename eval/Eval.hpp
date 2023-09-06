#pragma once

#include <ExLexer.hpp>
#include <EvalFactory.hpp>

EvalCommand* evalParse(const ExLexem& lexem, EvalFactory& factory);

bool evalParseSupported(const ExLexem& lexem);
