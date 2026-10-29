#pragma once

#include <QString>
#include <QColor>
#include "Token.h"

/**
 * @brief Manages colors, typography, badges, and stylesheets
 * calibrated from the "TINY Lexical Analyzer Studio" design system.
 */
class Theme {
public:
    // Core Palette Colors
    static const QString COLOR_BACKGROUND;
    static const QString COLOR_SURFACE_LOW;
    static const QString COLOR_SURFACE_CONTAINER;
    static const QString COLOR_SURFACE_HIGH;
    static const QString COLOR_SURFACE_HIGHEST;
    static const QString COLOR_BORDER;
    static const QString COLOR_PRIMARY;
    static const QString COLOR_PRIMARY_HOVER;
    static const QString COLOR_PRIMARY_ACTIVE;
    static const QString COLOR_TEXT;
    static const QString COLOR_TEXT_MUTED;
    static const QString COLOR_ERROR;
    static const QString COLOR_ERROR_CONTAINER;
    static const QString COLOR_SUCCESS;

    // Token Category Colors
    static const QString COLOR_TOKEN_KEYWORD;
    static const QString COLOR_TOKEN_IDENTIFIER;
    static const QString COLOR_TOKEN_NUMBER;
    static const QString COLOR_TOKEN_OPERATOR;
    static const QString COLOR_TOKEN_DELIMITER;
    static const QString COLOR_TOKEN_ERROR;

    /**
     * @brief Generates the comprehensive application dark theme stylesheet.
     */
    static QString getApplicationStyleSheet();

    /**
     * @brief Generates the styled HTML chip/badge for a specific token type.
     */
    static QString getTokenBadgeHtml(TokenType type);

    /**
     * @brief Returns the text color for a token type.
     */
    static QColor getTokenColor(TokenType type);
};
