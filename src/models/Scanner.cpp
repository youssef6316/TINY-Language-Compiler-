#include "Scanner.h"
#include <cctype>
#include <algorithm>

Scanner::Scanner(std::string sourceCode) {
    initReservedWords();
    setSource(std::move(sourceCode));
}

void Scanner::initReservedWords() {
    m_reservedWords = {
        {"if",     TokenType::IF},
        {"then",   TokenType::THEN},
        {"else",   TokenType::ELSE},
        {"end",    TokenType::END},
        {"repeat", TokenType::REPEAT},
        {"until",  TokenType::UNTIL},
        {"read",   TokenType::READ},
        {"write",  TokenType::WRITE}
    };
}

void Scanner::setSource(std::string sourceCode) {
    m_source = std::move(sourceCode);
    reset();

    // Calculate total lines
    m_totalLines = 1;
    for (char ch : m_source) {
        if (ch == '\n') {
            m_totalLines++;
        }
    }
}

void Scanner::reset() {
    m_cursor = 0;
    m_currentLine = 1;
    m_currentCol = 1;
    m_errorCount = 0;
}

char Scanner::getNextChar() {
    if (m_cursor >= m_source.size()) {
        return '\0';
    }
    char c = m_source[m_cursor++];
    if (c == '\n') {
        m_currentLine++;
        m_currentCol = 1;
    } else {
        m_currentCol++;
    }
    return c;
}

void Scanner::ungetNextChar() {
    if (m_cursor == 0) return;
    m_cursor--;
    char c = m_source[m_cursor];
    if (c == '\n') {
        m_currentLine--;
        m_currentCol = 1;
        size_t p = m_cursor;
        while (p > 0 && m_source[p - 1] != '\n') {
            m_currentCol++;
            p--;
        }
    } else {
        m_currentCol--;
    }
}

char Scanner::peekNextChar() const {
    if (m_cursor >= m_source.size()) {
        return '\0';
    }
    return m_source[m_cursor];
}

TokenType Scanner::checkReserved(const std::string& lexeme) const {
    std::string lower = lexeme;
    std::transform(lower.begin(), lower.end(), lower.begin(),
                   [](unsigned char ch) { return std::tolower(ch); });

    auto it = m_reservedWords.find(lower);
    if (it != m_reservedWords.end()) {
        return it->second;
    }
    return TokenType::IDENTIFIER;
}

Token Scanner::getToken() {
    Token token;
    StateType state = StateType::START;
    bool saveChar = true;

    int startLine = m_currentLine;
    int startCol = m_currentCol;

    while (state != StateType::DONE) {
        char c = getNextChar();
        saveChar = true;

        switch (state) {
            case StateType::START: {
                if (c == '\0') {
                    saveChar = false;
                    state = StateType::DONE;
                    token.type = TokenType::ENDFILE;
                } else if (std::isspace(static_cast<unsigned char>(c))) {
                    saveChar = false;
                    startLine = m_currentLine;
                    startCol = m_currentCol;
                } else if (c == '{') {
                    saveChar = false;
                    state = StateType::INCOMMENT_CURLY;
                } else if (c == '/' && peekNextChar() == '/') {
                    saveChar = false;
                    getNextChar(); // consume second '/'
                    state = StateType::INCOMMENT_LINE;
                } else if (c == ':') {
                    state = StateType::INASSIGN;
                } else if (std::isdigit(static_cast<unsigned char>(c))) {
                    state = StateType::INNUM;
                } else if (std::isalpha(static_cast<unsigned char>(c))) {
                    state = StateType::INID;
                } else if (c == ';') {
                    state = StateType::DONE;
                    token.type = TokenType::SEMICOLON;
                } else if (c == '<') {
                    state = StateType::DONE;
                    token.type = TokenType::LESSTHAN;
                } else if (c == '=') {
                    state = StateType::DONE;
                    token.type = TokenType::EQUAL;
                } else if (c == '+') {
                    state = StateType::DONE;
                    token.type = TokenType::PLUS;
                } else if (c == '-') {
                    state = StateType::DONE;
                    token.type = TokenType::MINUS;
                } else if (c == '*') {
                    state = StateType::DONE;
                    token.type = TokenType::MULT;
                } else if (c == '/') {
                    state = StateType::DONE;
                    token.type = TokenType::DIV;
                } else if (c == '(') {
                    state = StateType::DONE;
                    token.type = TokenType::OPENBRACKET;
                } else if (c == ')') {
                    state = StateType::DONE;
                    token.type = TokenType::CLOSEDBRACKET;
                } else {
                    // Unknown / invalid character
                    state = StateType::DONE;
                    token.type = TokenType::UNKNOWN;
                    token.errorMessage = std::string("Unknown character '") + c + "'";
                }
                break;
            }

            case StateType::INCOMMENT_CURLY: {
                saveChar = false;
                if (c == '}') {
                    state = StateType::START;
                    startLine = m_currentLine;
                    startCol = m_currentCol;
                } else if (c == '\0') {
                    state = StateType::DONE;
                    token.type = TokenType::UNKNOWN;
                    token.errorMessage = "Unterminated comment: missing closing '}'";
                    token.value = "{...";
                    saveChar = false;
                }
                break;
            }

            case StateType::INCOMMENT_LINE: {
                saveChar = false;
                if (c == '\n' || c == '\0') {
                    if (c == '\0') ungetNextChar();
                    state = StateType::START;
                    startLine = m_currentLine;
                    startCol = m_currentCol;
                }
                break;
            }

            case StateType::INASSIGN: {
                state = StateType::DONE;
                if (c == '=') {
                    token.type = TokenType::ASSIGN;
                } else {
                    // colon without '=' is an error in TINY
                    ungetNextChar();
                    saveChar = false;
                    token.type = TokenType::UNKNOWN;
                    token.errorMessage = "Invalid operator: expected '=' after ':'";
                }
                break;
            }

            case StateType::INNUM: {
                if (!std::isdigit(static_cast<unsigned char>(c))) {
                    ungetNextChar();
                    saveChar = false;
                    state = StateType::DONE;
                    token.type = TokenType::NUMBER;
                }
                break;
            }

            case StateType::INID: {
                if (!std::isalnum(static_cast<unsigned char>(c))) {
                    ungetNextChar();
                    saveChar = false;
                    state = StateType::DONE;
                    token.type = TokenType::IDENTIFIER;
                }
                break;
            }

            case StateType::DONE:
            default:
                state = StateType::DONE;
                break;
        }

        if (saveChar && c != '\0') {
            token.value += c;
        }
    }

    token.line = startLine;
    token.column = startCol;

    // Check reserved keywords
    if (token.type == TokenType::IDENTIFIER) {
        token.type = checkReserved(token.value);
    }

    if (token.isError()) {
        m_errorCount++;
    }

    return token;
}

std::vector<Token> Scanner::tokenizeAll(bool includeEndfile) {
    reset();
    std::vector<Token> tokens;

    while (true) {
        Token tok = getToken();
        if (tok.type == TokenType::ENDFILE) {
            if (includeEndfile) {
                tokens.push_back(tok);
            }
            break;
        }
        tokens.push_back(tok);
    }

    return tokens;
}
