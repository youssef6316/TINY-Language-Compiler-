#pragma once

#include <QString>
#include <vector>
#include "Token.h"
#include "Scanner.h"

/**
 * @brief Controller in the MVC pattern that coordinates between the Scanner model
 * and the MainWindow view. Manages file I/O, scan execution, and performance metrics.
 */
class MainController {
public:
    MainController() = default;

    /**
     * @brief Loads source code from a file into content string.
     */
    bool loadFile(const QString &filePath, QString &outContent, QString &outError);

    /**
     * @brief Exports tokens to a text file in accordance with project specifications:
     * Format: "<tokenvalue> , <tokentype>"
     */
    bool exportTokensToFile(const QString &filePath, const std::vector<Token> &tokens, QString &outError);

    /**
     * @brief Executes the lexical analysis on the given source code.
     * Computes tokens, counts errors, and measures elapsed time in milliseconds.
     */
    void scanSource(const QString &sourceCode,
                    std::vector<Token> &outTokens,
                    int &outErrorCount,
                    double &outElapsedMs);
};
