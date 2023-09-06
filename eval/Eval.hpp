#pragma once

#include <vector>

struct Block;
struct SyntaxTree;
struct Diagnostic;

bool evalParseBlock(Block& block, SyntaxTree& root, std::vector<Diagnostic>& digs);
