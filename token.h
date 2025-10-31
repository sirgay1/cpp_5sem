#pragma once
#include <string>

enum class TokenKind {
    Number,
    Plus, Minus, Mul, Div,
    LParen, RParen,
    Comma,
    Identifier,
    End
};

struct Token {
    TokenKind kind;
    double value;
    std::string text;
};
