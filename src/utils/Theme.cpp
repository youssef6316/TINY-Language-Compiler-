#include "Theme.h"

const QString Theme::COLOR_BACKGROUND         = "#131313";
const QString Theme::COLOR_SURFACE_LOW        = "#1B1B1C";
const QString Theme::COLOR_SURFACE_CONTAINER  = "#202020";
const QString Theme::COLOR_SURFACE_HIGH       = "#2A2A2A";
const QString Theme::COLOR_SURFACE_HIGHEST    = "#353535";
const QString Theme::COLOR_BORDER             = "#333333";
const QString Theme::COLOR_PRIMARY            = "#007ACC";
const QString Theme::COLOR_PRIMARY_HOVER      = "#0062A3";
const QString Theme::COLOR_PRIMARY_ACTIVE     = "#005085";
const QString Theme::COLOR_TEXT               = "#E5E2E1";
const QString Theme::COLOR_TEXT_MUTED         = "#8A919D";
const QString Theme::COLOR_ERROR              = "#FFB4AB";
const QString Theme::COLOR_ERROR_CONTAINER    = "#93000A";
const QString Theme::COLOR_SUCCESS            = "#61DAC1";

const QString Theme::COLOR_TOKEN_KEYWORD      = "#9FCAFF";
const QString Theme::COLOR_TOKEN_IDENTIFIER   = "#61DAC1";
const QString Theme::COLOR_TOKEN_NUMBER       = "#F6BB84";
const QString Theme::COLOR_TOKEN_OPERATOR     = "#F6BB84";
const QString Theme::COLOR_TOKEN_DELIMITER    = "#C0C7D3";
const QString Theme::COLOR_TOKEN_ERROR        = "#FFB4AB";

QString Theme::getApplicationStyleSheet() {
    return QString(R"(
        QWidget {
            background-color: %1;
            color: %2;
            font-family: "Inter", "Segoe UI", -apple-system, BlinkMacSystemFont, sans-serif;
            font-size: 13px;
        }

        QMainWindow {
            background-color: %1;
        }

        /* Menu Bar */
        QMenuBar {
            background-color: %3;
            color: %2;
            border-bottom: 1px solid %4;
            padding: 3px 8px;
            font-size: 12px;
            font-weight: 500;
        }
        QMenuBar::item {
            background: transparent;
            padding: 4px 10px;
            border-radius: 4px;
            margin-right: 2px;
        }
        QMenuBar::item:selected {
            background-color: %5;
            color: #FFFFFF;
        }

        QMenu {
            background-color: %3;
            color: %2;
            border: 1px solid %4;
            border-radius: 6px;
            padding: 5px;
        }
        QMenu::item {
            padding: 6px 26px 6px 18px;
            border-radius: 4px;
        }
        QMenu::item:selected {
            background-color: %6;
            color: #FFFFFF;
        }
        QMenu::separator {
            height: 1px;
            background: %4;
            margin: 4px 6px;
        }

        /* Toolbar */
        QToolBar {
            background-color: %7;
            border-bottom: 1px solid %4;
            padding: 6px 14px;
            spacing: 10px;
        }

        /* Standard Push Button */
        QPushButton {
            background-color: %8;
            color: %2;
            border: 1px solid %4;
            border-radius: 5px;
            padding: 5px 14px;
            font-weight: 500;
            font-size: 12px;
        }
        QPushButton:hover {
            background-color: %5;
            border-color: #4E4E52;
            color: #FFFFFF;
        }
        QPushButton:pressed {
            background-color: #171717;
        }

        /* Primary Hero Button: Scan Code */
        QPushButton#primaryActionBtn {
            background-color: %6;
            color: #FFFFFF;
            border: 1px solid %6;
            border-radius: 5px;
            padding: 5px 16px;
            font-weight: 600;
            font-size: 12px;
        }
        QPushButton#primaryActionBtn:hover {
            background-color: %9;
            border-color: %9;
        }
        QPushButton#primaryActionBtn:pressed {
            background-color: %10;
        }

        /* Alerting Red Button: Clear */
        QPushButton#clearActionBtn {
            background-color: #381518;
            color: #FFA4A8;
            border: 1px solid #78262C;
            border-radius: 5px;
            padding: 5px 14px;
            font-weight: 600;
            font-size: 12px;
        }
        QPushButton#clearActionBtn:hover {
            background-color: #551D22;
            border-color: #A3333C;
            color: #FFFFFF;
        }
        QPushButton#clearActionBtn:pressed {
            background-color: #270C0E;
            border-color: #78262C;
        }

        /* Right-Corner Export Tokens Button */
        QPushButton#exportActionBtn {
            background-color: #1E293B;
            color: #93C5FD;
            border: 1px solid #3B82F6;
            border-radius: 5px;
            padding: 5px 16px;
            font-weight: 600;
            font-size: 12px;
        }
        QPushButton#exportActionBtn:hover {
            background-color: #2563EB;
            border-color: #60A5FA;
            color: #FFFFFF;
        }
        QPushButton#exportActionBtn:pressed {
            background-color: #1D4ED8;
        }

        /* Search Filter Input */
        QLineEdit {
            background-color: %8;
            border: 1px solid %4;
            border-radius: 5px;
            padding: 4px 10px;
            color: %2;
            font-family: "JetBrains Mono", "Consolas", monospace;
            font-size: 12px;
        }
        QLineEdit:focus {
            border: 1px solid %6;
            background-color: %7;
        }

        /* Splitter */
        QSplitter::handle {
            background-color: %4;
        }
        QSplitter::handle:hover {
            background-color: %6;
        }

        /* Table Widget */
        QTableWidget {
            background-color: %1;
            alternate-background-color: #171718;
            border: none;
            gridline-color: #242426;
            color: %2;
            selection-background-color: #094771;
            selection-color: #FFFFFF;
            font-family: "JetBrains Mono", "Consolas", monospace;
            font-size: 12px;
        }
        QTableWidget::item {
            padding: 4px 8px;
        }

        QHeaderView::section {
            background-color: %3;
            color: %11;
            padding: 6px 10px;
            border: none;
            border-right: 1px solid %4;
            border-bottom: 1px solid %4;
            font-size: 11px;
            font-weight: 600;
            text-transform: uppercase;
            letter-spacing: 0.04em;
        }

        /* Scrollbars */
        QScrollBar:vertical {
            background: %1;
            width: 10px;
            margin: 0px;
        }
        QScrollBar::handle:vertical {
            background: %8;
            min-height: 24px;
            border-radius: 4px;
        }
        QScrollBar::handle:vertical:hover {
            background: %5;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }
        QScrollBar:horizontal {
            background: %1;
            height: 10px;
            margin: 0px;
        }
        QScrollBar::handle:horizontal {
            background: %8;
            min-width: 24px;
            border-radius: 4px;
        }
        QScrollBar::handle:horizontal:hover {
            background: %5;
        }
        QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
            width: 0px;
        }

        /* Status Bar */
        QStatusBar {
            background-color: #171718;
            color: %11;
            border-top: 1px solid %4;
            font-family: "JetBrains Mono", "Consolas", monospace;
            font-size: 11px;
            font-weight: 500;
            padding: 2px 6px;
        }
        QStatusBar QLabel {
            background: transparent;
            color: %2;
            padding: 2px 6px;
        }

        /* Tooltip */
        QToolTip {
            background-color: %3;
            color: %2;
            border: 1px solid %4;
            border-radius: 4px;
            padding: 5px 9px;
            font-size: 12px;
        }
    )")
    .arg(COLOR_BACKGROUND)         // %1
    .arg(COLOR_TEXT)               // %2
    .arg(COLOR_SURFACE_HIGH)       // %3
    .arg(COLOR_BORDER)             // %4
    .arg(COLOR_SURFACE_HIGHEST)    // %5
    .arg(COLOR_PRIMARY)            // %6
    .arg(COLOR_SURFACE_LOW)        // %7
    .arg(COLOR_SURFACE_CONTAINER)  // %8
    .arg(COLOR_PRIMARY_HOVER)      // %9
    .arg(COLOR_PRIMARY_ACTIVE)     // %10
    .arg(COLOR_TEXT_MUTED);        // %11
}

QString Theme::getTokenBadgeHtml(TokenType type) {
    QString bg;
    QString fg;
    QString border;
    QString label = QString::fromStdString(tokenTypeToString(type));

    switch (type) {
        case TokenType::IF:
        case TokenType::THEN:
        case TokenType::ELSE:
        case TokenType::END:
        case TokenType::REPEAT:
        case TokenType::UNTIL:
        case TokenType::READ:
        case TokenType::WRITE:
            bg = "rgba(0, 122, 204, 0.22)";
            fg = COLOR_TOKEN_KEYWORD;
            border = "rgba(159, 202, 255, 0.40)";
            break;

        case TokenType::IDENTIFIER:
            bg = "rgba(0, 134, 114, 0.22)";
            fg = COLOR_TOKEN_IDENTIFIER;
            border = "rgba(97, 218, 193, 0.40)";
            break;

        case TokenType::NUMBER:
            bg = "rgba(104, 64, 20, 0.28)";
            fg = COLOR_TOKEN_NUMBER;
            border = "rgba(246, 187, 132, 0.40)";
            break;

        case TokenType::ASSIGN:
        case TokenType::LESSTHAN:
        case TokenType::EQUAL:
        case TokenType::PLUS:
        case TokenType::MINUS:
        case TokenType::MULT:
        case TokenType::DIV:
            bg = "rgba(104, 64, 20, 0.28)";
            fg = COLOR_TOKEN_OPERATOR;
            border = "rgba(246, 187, 132, 0.40)";
            break;

        case TokenType::SEMICOLON:
        case TokenType::OPENBRACKET:
        case TokenType::CLOSEDBRACKET:
            bg = "#262628";
            fg = COLOR_TOKEN_DELIMITER;
            border = "#404751";
            break;

        case TokenType::UNKNOWN:
        default:
            bg = "rgba(147, 0, 10, 0.35)";
            fg = COLOR_TOKEN_ERROR;
            border = "rgba(255, 180, 171, 0.50)";
            break;
    }

    return QString("<span style=\"background-color:%1; color:%2; border:1px solid %3; "
                   "border-radius:4px; padding:2px 8px; font-weight:600; font-size:11px;\">"
                   "%4</span>")
           .arg(bg, fg, border, label);
}

QColor Theme::getTokenColor(TokenType type) {
    switch (type) {
        case TokenType::IF:
        case TokenType::THEN:
        case TokenType::ELSE:
        case TokenType::END:
        case TokenType::REPEAT:
        case TokenType::UNTIL:
        case TokenType::READ:
        case TokenType::WRITE:
            return QColor(COLOR_TOKEN_KEYWORD);
        case TokenType::IDENTIFIER:
            return QColor(COLOR_TOKEN_IDENTIFIER);
        case TokenType::NUMBER:
            return QColor(COLOR_TOKEN_NUMBER);
        case TokenType::ASSIGN:
        case TokenType::LESSTHAN:
        case TokenType::EQUAL:
        case TokenType::PLUS:
        case TokenType::MINUS:
        case TokenType::MULT:
        case TokenType::DIV:
            return QColor(COLOR_TOKEN_OPERATOR);
        case TokenType::SEMICOLON:
        case TokenType::OPENBRACKET:
        case TokenType::CLOSEDBRACKET:
            return QColor(COLOR_TOKEN_DELIMITER);
        case TokenType::UNKNOWN:
        default:
            return QColor(COLOR_TOKEN_ERROR);
    }
}

