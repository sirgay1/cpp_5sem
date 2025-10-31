#include "calculator.h"
#include <cctype>
#include <cmath>
#include <stdexcept>
#include <iostream>
#include <algorithm>

using namespace std;

Calculator::Calculator(const PluginManager& pm) : plugins(pm) {}

Token Calculator::getNextToken() {
    while (pos < input.size() && isspace(input[pos])) pos++;
    if (pos >= input.size()) return { TokenKind::End, 0, "" };

    char c = input[pos];

    if (isdigit(c) || c == '.') {
        size_t start = pos;
        while (pos < input.size() && (isdigit(input[pos]) || input[pos] == '.')) pos++;
        double val = stod(input.substr(start, pos - start));
        return { TokenKind::Number, val, "" };
    }

    pos++;
    switch (c) {
    case '+': return { TokenKind::Plus };
    case '-': return { TokenKind::Minus };
    case '*': return { TokenKind::Mul };
    case '/': return { TokenKind::Div };
    case '(': return { TokenKind::LParen };
    case ')': return { TokenKind::RParen };
    case ',': return { TokenKind::Comma };
    default:
        if (isalpha(c)) {
            size_t start = pos - 1;
            while (pos < input.size() && (isalpha(input[pos]) || isdigit(input[pos]))) pos++;
            string text = input.substr(start, pos - start);
            return { TokenKind::Identifier, 0, text };
        }
        throw runtime_error(string("Unknown character: ") + c);
    }
}

double Calculator::evaluate(const string& expr) {
    input = expr;
    pos = 0;
    double res = parseExpression();

    Token t = getNextToken();
    if (t.kind != TokenKind::End)
        throw runtime_error("Unexpected characters at end of expression");

    return res;
}

double Calculator::parseExpression() {
    double left = parseTerm();
    while (true) {
        size_t backup = pos;
        Token t = getNextToken();
        if (t.kind == TokenKind::Plus)
            left += parseTerm();
        else if (t.kind == TokenKind::Minus)
            left -= parseTerm();
        else {
            pos = backup;
            break;
        }
    }
    return left;
}

double Calculator::parseTerm() {
    double left = parseFactor();
    while (true) {
        size_t backup = pos;
        Token t = getNextToken();
        if (t.kind == TokenKind::Mul)
            left *= parseFactor();
        else if (t.kind == TokenKind::Div) {
            double r = parseFactor();
            if (r == 0) throw runtime_error("Division by zero");
            left /= r;
        }
        else {
            pos = backup;
            break;
        }
    }
    return left;
}

double Calculator::parseFactor() {
    return parsePrimary();
}

double Calculator::parsePrimary() {
    Token t = getNextToken();

    if (t.kind == TokenKind::Number)
        return t.value;

    if (t.kind == TokenKind::Minus)
        return -parsePrimary();

    if (t.kind == TokenKind::LParen) {
        double val = parseExpression();
        Token r = getNextToken();
        if (r.kind != TokenKind::RParen)
            throw runtime_error("Missing ')'");
        return val;
    }

    if (t.kind == TokenKind::Identifier) {
        string fname = t.text;
        transform(fname.begin(), fname.end(), fname.begin(), ::tolower);

        Token next = getNextToken();
        if (next.kind != TokenKind::LParen)
            throw runtime_error("Expected '(' after function name");

        vector<double> args;

        size_t backup = pos;
        Token lookahead = getNextToken();
        if (lookahead.kind == TokenKind::RParen) {
        }
        else {
            pos = backup;
            args.push_back(parseExpression());
            while (true) {
                Token comma = getNextToken();
                if (comma.kind == TokenKind::RParen)
                    break;
                if (comma.kind != TokenKind::Comma)
                    throw runtime_error("Expected ',' or ')'");
                args.push_back(parseExpression());
            }
        }

        return plugins.callFunction(fname, args);
    }

    throw runtime_error("Unexpected token in expression");
}
