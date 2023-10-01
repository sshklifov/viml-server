#pragma once

#include "ExLexem.hpp"

int do_one_cmd(const char* cmdline, ExLexem& lexem);

/// Check for an Ex command with optional tail.
/// If there is a match advance "pp" to the argument and return true.
///
/// @param pp   start of command
/// @param cmd  name of command
/// @param len  required length
bool checkforcmd(const char*& pp, const char* cmd, int len);

/// Check if "c" is:
/// - end of command
/// - comment
/// - command separator
int ends_excmd(int c);

/// Same as "ends_excmd" but don't accept comments (might be expression)
int ends_notrlcom(int c);
