#pragma once

#include "ExLexem.hpp"
#include "DiagnosticReporter.hpp"

int do_one_cmd(const char* cmdline,  const char*& nextcmd, ExLexem& lexem);
