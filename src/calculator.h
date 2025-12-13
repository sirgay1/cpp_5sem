#pragma once
#include "plugin_manager.h"
#include "token.h"
#include <string>

class Calculator {
public:
    Calculator(const PluginManager& pm);
    double evaluate(const std::string& expr);
private:
    const PluginManager& plugins;
    size_t pos;
    std::string input;

    Token getNextToken();
    double parseExpression();
    double parseTerm();
    double parseFactor();
    double parsePrimary();
};
