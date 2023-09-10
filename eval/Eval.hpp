#pragma once

#include "EvalFactory.hpp"
#include "EvalCommand.hpp"

#include <Blocks.hpp>
#include <DiagnosticReporter.hpp>

EvalCommand* evalEx(const ExLexem& lexem, EvalFactory& factory, DiagnosticReporter& reporter);
