#include "Token.h"

std::string Token::toExportString() const {
    return value + " , " + tokenTypeToString(type);
}

std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::ENDFILE:      return "ENDFILE";
        case TokenType::IF:           return "IF";
        case TokenType::THEN:         return "THEN";
        case TokenType::ELSE:         return "ELSE";
        case TokenType::END:          return "END";
        case TokenType::REPEAT:       return "REPEAT";
        case TokenType::UNTIL:        return "UNTIL";
        case TokenType::READ:         return "READ";
        case TokenType::WRITE:        return "WRITE";
        case TokenType::IDENTIFIER:   return "IDENTIFIER";
        case TokenType::NUMBER:       return "NUMBER";
        case TokenType::ASSIGN:       return "ASSIGN";
        case TokenType::EQUAL:        return "EQUAL";
        case TokenType::LESSTHAN:     return "LESSTHAN";
        case TokenType::PLUS:         return "PLUS";
        case TokenType::MINUS:        return "MINUS";
        case TokenType::MULT:         return "MULT";
        case TokenType::DIV:          return "DIV";
        case TokenType::SEMICOLON:    return "SEMICOLON";
        case TokenType::OPENBRACKET:  return "OPENBRACKET";
        case TokenType::CLOSEDBRACKET:return "CLOSEDBRACKET";
        case TokenType::UNKNOWN:      return "UNKNOWN";
        default:                      return "UNKNOWN";
    }
}

std::string tokenCategoryToString(TokenType type) {
    switch (type) {
        case TokenType::IF:
        case TokenType::THEN:
        case TokenType::ELSE:
        case TokenType::END:
        case TokenType::REPEAT:
        case TokenType::UNTIL:
        case TokenType::READ:
        case TokenType::WRITE:
            return "KEYWORD";
        case TokenType::IDENTIFIER:
            return "IDENTIFIER";
        case TokenType::NUMBER:
            return "NUMBER";
        case TokenType::ASSIGN:
        case TokenType::EQUAL:
        case TokenType::LESSTHAN:
        case TokenType::PLUS:
        case TokenType::MINUS:
        case TokenType::MULT:
        case TokenType::DIV:
            return "OPERATOR";
        case TokenType::SEMICOLON:
        case TokenType::OPENBRACKET:
        case TokenType::CLOSEDBRACKET:
            return "DELIMITER";
        case TokenType::ENDFILE:
            return "ENDFILE";
        case TokenType::UNKNOWN:
        default:
            return "ERROR";
    }
}

std::string tokenDescription(TokenType type, const std::string& lexeme) {
    switch (type) {
        case TokenType::IF:           return "Reserved Keyword: 'if'";
        case TokenType::THEN:         return "Reserved Keyword: 'then'";
        case TokenType::ELSE:         return "Reserved Keyword: 'else'";
        case TokenType::END:          return "Reserved Keyword: 'end'";
        case TokenType::REPEAT:       return "Reserved Keyword: 'repeat'";
        case TokenType::UNTIL:        return "Reserved Keyword: 'until'";
        case TokenType::READ:         return "Reserved Keyword: 'read'";
        case TokenType::WRITE:        return "Reserved Keyword: 'write'";
        case TokenType::IDENTIFIER:   return "User Variable / Identifier: '" + lexeme + "'";
        case TokenType::NUMBER:       return "Integer Literal: " + lexeme;
        case TokenType::ASSIGN:       return "Assignment Operator ':='";
        case TokenType::EQUAL:        return "Comparison Operator '='";
        case TokenType::LESSTHAN:     return "Comparison Operator '<'";
        case TokenType::PLUS:         return "Arithmetic Operator '+'";
        case TokenType::MINUS:        return "Arithmetic Operator '-'";
        case TokenType::MULT:         return "Arithmetic Operator '*'";
        case TokenType::DIV:          return "Arithmetic Operator '/'";
        case TokenType::SEMICOLON:    return "Statement Terminator ';'";
        case TokenType::OPENBRACKET:  return "Grouping Open Bracket '('";
        case TokenType::CLOSEDBRACKET:return "Grouping Close Bracket ')'";
        case TokenType::ENDFILE:      return "End of Input Stream";
        case TokenType::UNKNOWN:      return "Unknown Character / Lexical Error";
        default:                      return "Undefined Token";
    }
}
