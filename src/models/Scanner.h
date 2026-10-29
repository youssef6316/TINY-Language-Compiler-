#pragma once

#include "Token.h"
#include <string>
#include <vector>
#include <unordered_map>

/**
 * @brief States for the TINY Scanner Finite Automaton (DFA).
 */
enum class StateType {
    START,
    INASSIGN,
    INCOMMENT_CURLY,
    INCOMMENT_LINE,
    INNUM,
    INID,
    DONE
};

/**
 * @brief Lexical Analyzer (Scanner) for the TINY Language.
 * Implements a deterministic finite state machine to tokenize input streams.
 */
class Scanner {
public:
    explicit Scanner(std::string sourceCode = "");

    /**
     * @brief Sets or replaces the source code buffer and resets the scanner state.
     */
    void setSource(std::string sourceCode);

    /**
     * @brief Resets the scanner to the beginning of the current source buffer.
     */
    void reset();

    /**
     * @brief Core lexical analyzer function: extracts and returns the next token.
     * Computes attributes such as token type, lexeme string value, line, and column.
     * Required by project specifications.
     */
    Token getToken();

    /**
     * @brief Scans the entire source buffer from start to finish.
     * Returns all recognized tokens (excluding ENDFILE, unless desired).
     */
    std::vector<Token> tokenizeAll(bool includeEndfile = false);

    /**
     * @brief Returns the total count of lexical errors encountered in the last full scan.
     */
    int getErrorCount() const { return m_errorCount; }

    /**
     * @brief Returns the total number of lines in the source buffer.
     */
    int getTotalLines() const { return m_totalLines; }

    /**
     * @brief Returns the total character count in the source buffer.
     */
    size_t getTotalChars() const { return m_source.size(); }

private:
    char getNextChar();
    void ungetNextChar();
    char peekNextChar() const;

    TokenType checkReserved(const std::string& lexeme) const;

    std::string m_source;
    size_t m_cursor{0};
    int m_currentLine{1};
    int m_currentCol{1};

    int m_errorCount{0};
    int m_totalLines{1};

    std::unordered_map<std::string, TokenType> m_reservedWords;
    void initReservedWords();
};

