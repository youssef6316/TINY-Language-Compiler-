#pragma once

#include <QMainWindow>
#include <QSplitter>
#include <QTableWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <vector>
#include "Token.h"
#include "CodeEditor.h"
#include "MainController.h"

/**
 * @brief Main View window for the TINY Lexical Analyzer Studio.
 * Integrates split-pane layout, code editor, token visualization table,
 * diagnostics ribbon, toolbar actions, and file dialogs.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

private slots:
    // Core Actions
    void onOpenFile();
    void onScanCode();
    void onClearAll();
    void onExportTokens();

    // Filter & Diagnostics
    void onFilterTextChanged(const QString &text);
    void onTableItemDoubleClicked(int row, int column);
    void onGoToErrorLineClicked();
    void onNextErrorClicked();
    void onPrevErrorClicked();

    // Editor signals
    void onCursorInfoChanged(int line, int col);
    void onEditorStatsChanged(int lines, int chars);

    // Help Dialogs
    void onAbout();
    void onTokenGrammarHelp();

private:
    void setupUi();
    void setupMenuBar();
    void setupToolBar();
    QWidget* createLeftPane();
    QWidget* createRightPane();
    void setupStatusBar();

    void updateTokenTable();
    void updateDiagnosticsRibbon();
    void updateStatusBarMetrics(double elapsedMs = 0.0);

    // MVC Controller
    MainController m_controller;

    // Data State
    std::vector<Token> m_currentTokens;
    std::vector<size_t> m_errorTokenIndices;
    int m_currentErrorIndex{-1};
    QString m_currentFilePath;

    // UI Widgets
    QSplitter *m_mainSplitter{nullptr};
    CodeEditor *m_editor{nullptr};
    QLabel *m_fileTabLabel{nullptr};
    QLabel *m_editorStatsLabel{nullptr};

    // Right Pane UI
    QLabel *m_tokenCountBadge{nullptr};
    QLineEdit *m_filterEdit{nullptr};
    QTableWidget *m_tableWidget{nullptr};

    // Diagnostics Ribbon
    QWidget *m_diagnosticsRibbon{nullptr};
    QLabel *m_diagWarningIcon{nullptr};
    QLabel *m_diagMessageLabel{nullptr};
    QLabel *m_diagCodeBadge{nullptr};
    QPushButton *m_goToLineBtn{nullptr};
    QPushButton *m_prevErrorBtn{nullptr};
    QPushButton *m_nextErrorBtn{nullptr};
    QLabel *m_errorCounterLabel{nullptr};

    // Status Bar Labels
    QLabel *m_statusLeftLabel{nullptr};
    QLabel *m_statusCursorLabel{nullptr};
    QLabel *m_statusTokensLabel{nullptr};
    QLabel *m_statusErrorsLabel{nullptr};
};
