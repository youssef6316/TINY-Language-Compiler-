#pragma once

#include <string>

/**
 * @brief Represents the types of tokens recognized in the TINY Language.
 * As defined in Louden's TINY language specification and course requirements.
 */
enum class TokenType {
    // End of File
    ENDFILE,

    // Reserved Keywords
    IF,
    THEN,
    ELSE,
    END,
    REPEAT,
    UNTIL,
    READ,
    WRITE,

    // User Identifiers & Literals
    IDENTIFIER,
    NUMBER,

    // Special Symbols / Operators
    ASSIGN,         // :=
    EQUAL,          // =
    LESSTHAN,       // <
    PLUS,           // +
    MINUS,          // -
    MULT,           // *
    DIV,            // /
    SEMICOLON,      // ;
    OPENBRACKET,    // (
    CLOSEDBRACKET,  // )

    // Lexical Error / Invalid Character Sequence
    UNKNOWN
};

/**
 * @brief Represents a recognized token with its lexeme, type, and source position.
 */
struct Token {
    TokenType type{TokenType::UNKNOWN};
    std::string value;            // Lexeme string value
    int line{1};                  // 1-based source line
    int column{1};                // 1-based source column
    std::string errorMessage;     // Populated if type == TokenType::UNKNOWN

    /**
     * @brief Checks if this token is a lexical error.
     */
    bool isError() const {
        return type == TokenType::UNKNOWN;
    }

    /**
     * @brief Formats the token for export to file: "<tokenvalue> , <tokentype>"
     * According to project specification requirements.
     */
    std::string toExportString() const;
};

/**
 * @brief Converts TokenType enum to uppercase canonical string representation.
 */
std::string tokenTypeToString(TokenType type);

/**
 * @brief Returns a broad category string for the token (Keyword, Identifier, etc.).
 */
std::string tokenCategoryToString(TokenType type);

/**
 * @brief Returns an explanatory description for the token (used in the UI details column).
 */
std::string tokenDescription(TokenType type, const std::string& lexeme);

