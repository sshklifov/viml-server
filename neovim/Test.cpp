#include "Eval.hpp"

int main() {
    const char* expr = "{a, b -> 3}";
    EvalFactory factory;

    Evaluator e;
    e.setFactory(&factory);
    int numRead = 0;
    EvalExpr* res = e.eval(expr, numRead);

    FStr s;
    res->appendStr(s);
}
