#include "MainWindow.h"
#include "Theme.h"
#include <QApplication>
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("TINY Language Lexical Analyzer Studio");
    resize(1280, 800);

    setupUi();
    setupMenuBar();
    setupToolBar();
    setupStatusBar();

    // Editor starts empty and clean, ready for file load or copy-paste input
    m_editor->setPlaceholderText("Write or paste TINY language code here, or click 'Open File' to load a source file...\n\n"
                                "Inputs accepted:\n"
                                "1. Open File via dialog (.tny, .txt)\n"
                                "2. Directly type or paste code into this editor\n\n"
                                "Click 'Scan Code' (or press F5) to run lexical analysis.");
}

void MainWindow::setupUi() {
    auto *centralWidget = new QWidget(this);
    auto *rootLayout = new QVBoxLayout(centralWidget);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    // Splitter for vertically split window in halves (left and right) with dynamic resizing
    m_mainSplitter = new QSplitter(Qt::Horizontal, centralWidget);
    m_mainSplitter->setHandleWidth(5);

    QWidget *leftPane = createLeftPane();
    QWidget *rightPane = createRightPane();

    m_mainSplitter->addWidget(leftPane);
    m_mainSplitter->addWidget(rightPane);

    // 50% / 50% initial split
    m_mainSplitter->setSizes({640, 640});

    rootLayout->addWidget(m_mainSplitter);
    setCentralWidget(centralWidget);
}

void MainWindow::setupMenuBar() {
    QMenuBar *menu = menuBar();

    // File Menu
    QMenu *fileMenu = menu->addMenu("&File");
    fileMenu->addAction("Open File...", QKeySequence::Open, this, &MainWindow::onOpenFile);
    fileMenu->addAction("Export Tokens...", QKeySequence::Save, this, &MainWindow::onExportTokens);
    fileMenu->addSeparator();
    fileMenu->addAction("Clear All", QKeySequence("Ctrl+L"), this, &MainWindow::onClearAll);
    fileMenu->addSeparator();
    fileMenu->addAction("Exit", QKeySequence::Quit, this, &QWidget::close);

    // Edit Menu
    QMenu *editMenu = menu->addMenu("&Edit");
    editMenu->addAction("Undo", QKeySequence::Undo, m_editor, &QPlainTextEdit::undo);
    editMenu->addAction("Redo", QKeySequence::Redo, m_editor, &QPlainTextEdit::redo);
    editMenu->addSeparator();
    editMenu->addAction("Cut", QKeySequence::Cut, m_editor, &QPlainTextEdit::cut);
    editMenu->addAction("Copy", QKeySequence::Copy, m_editor, &QPlainTextEdit::copy);
    editMenu->addAction("Paste", QKeySequence::Paste, m_editor, &QPlainTextEdit::paste);
    editMenu->addAction("Select All", QKeySequence::SelectAll, m_editor, &QPlainTextEdit::selectAll);

    // Lexer Menu
    QMenu *lexerMenu = menu->addMenu("&Lexer");
    lexerMenu->addAction("Scan Code (Run Lexer)", QKeySequence("F5"), this, &MainWindow::onScanCode);
    lexerMenu->addSeparator();
    lexerMenu->addAction("Clear Results", this, &MainWindow::onClearAll);

    // Help Menu
    QMenu *helpMenu = menu->addMenu("&Help");
    helpMenu->addAction("TINY Language Grammar...", this, &MainWindow::onTokenGrammarHelp);
    helpMenu->addSeparator();
    helpMenu->addAction("About Studio...", this, &MainWindow::onAbout);
}

void MainWindow::setupToolBar() {
    auto *toolbar = addToolBar("Main Actions");
    toolbar->setMovable(false);
    toolbar->setFloatable(false);

    // 1. Open File Button
    auto *openBtn = new QPushButton("Open File", this);
    openBtn->setToolTip("Open source code file (.tny, .txt) (Ctrl+O)");
    openBtn->setCursor(Qt::PointingHandCursor);
    connect(openBtn, &QPushButton::clicked, this, &MainWindow::onOpenFile);
    toolbar->addWidget(openBtn);

    // 2. Scan Code Button (Hero Trigger Center)
    auto *scanBtn = new QPushButton("Scan Code", this);
    scanBtn->setObjectName("primaryActionBtn");
    scanBtn->setToolTip("Execute lexical scanner and generate token stream (F5)");
    scanBtn->setCursor(Qt::PointingHandCursor);
    connect(scanBtn, &QPushButton::clicked, this, &MainWindow::onScanCode);
    toolbar->addWidget(scanBtn);

    // 3. Clear Button (Highlighted with Alerting Red Color)
    auto *clearBtn = new QPushButton("Clear", this);
    clearBtn->setObjectName("clearActionBtn");
    clearBtn->setToolTip("Clear source code and token results (Ctrl+L)");
    clearBtn->setCursor(Qt::PointingHandCursor);
    connect(clearBtn, &QPushButton::clicked, this, &MainWindow::onClearAll);
    toolbar->addWidget(clearBtn);

    // Spacer to push Export Tokens to the top-right corner
    auto *spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    toolbar->addWidget(spacer);

    // 4. Export Tokens Button (Replaces the top-right signature)
    auto *exportBtn = new QPushButton("Export Tokens", this);
    exportBtn->setObjectName("exportActionBtn");
    exportBtn->setToolTip("Export token stream to text file (Ctrl+S)");
    exportBtn->setCursor(Qt::PointingHandCursor);
    connect(exportBtn, &QPushButton::clicked, this, &MainWindow::onExportTokens);
    toolbar->addWidget(exportBtn);
}

QWidget* MainWindow::createLeftPane() {
    auto *leftContainer = new QWidget(this);
    auto *layout = new QVBoxLayout(leftContainer);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // Header Tab
    auto *header = new QWidget(leftContainer);
    header->setFixedHeight(36);
    header->setStyleSheet("background-color: #1E1E22; border-bottom: 1px solid #2E2E32;");
    auto *headerLayout = new QHBoxLayout(header);
    headerLayout->setContentsMargins(14, 0, 14, 0);

    m_fileTabLabel = new QLabel("untitled.tny  •  Source Code (.tny / .txt)", header);
    m_fileTabLabel->setStyleSheet("color: #E5E2E1; font-weight: 600; font-size: 12px;");
    headerLayout->addWidget(m_fileTabLabel);

    headerLayout->addStretch();

    m_editorStatsLabel = new QLabel("0 lines • 0 chars", header);
    m_editorStatsLabel->setStyleSheet("color: #8A919D; font-family: 'JetBrains Mono', monospace; font-size: 11px;");
    headerLayout->addWidget(m_editorStatsLabel);

    layout->addWidget(header);

    // Code Editor
    m_editor = new CodeEditor(leftContainer);
    connect(m_editor, &CodeEditor::cursorInfoChanged, this, &MainWindow::onCursorInfoChanged);
    connect(m_editor, &CodeEditor::statsChanged, this, &MainWindow::onEditorStatsChanged);
    layout->addWidget(m_editor);

    return leftContainer;
}

QWidget* MainWindow::createRightPane() {
    auto *rightContainer = new QWidget(this);
    auto *layout = new QVBoxLayout(rightContainer);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // Header with Title, Badge, and Filter Input
    auto *header = new QWidget(rightContainer);
    header->setFixedHeight(36);
    header->setStyleSheet("background-color: #1E1E22; border-bottom: 1px solid #2E2E32;");
    auto *headerLayout = new QHBoxLayout(header);
    headerLayout->setContentsMargins(14, 0, 14, 0);
    headerLayout->setSpacing(10);

    auto *titleLabel = new QLabel("Token Stream & Analysis Results", header);
    titleLabel->setStyleSheet("color: #E5E2E1; font-weight: 600; font-size: 12px;");
    headerLayout->addWidget(titleLabel);

    m_tokenCountBadge = new QLabel("0 Tokens Found", header);
    m_tokenCountBadge->setStyleSheet("background-color: rgba(97, 218, 193, 0.15); color: #61DAC1; "
                                     "border: 1px solid rgba(97, 218, 193, 0.35); border-radius: 10px; "
                                     "padding: 2px 10px; font-size: 11px; font-weight: 600;");
    headerLayout->addWidget(m_tokenCountBadge);

    headerLayout->addStretch();

    m_filterEdit = new QLineEdit(header);
    m_filterEdit->setPlaceholderText("Filter tokens...");
    m_filterEdit->setFixedWidth(190);
    connect(m_filterEdit, &QLineEdit::textChanged, this, &MainWindow::onFilterTextChanged);
    headerLayout->addWidget(m_filterEdit);

    layout->addWidget(header);

    // Results Table
    m_tableWidget = new QTableWidget(0, 4, rightContainer);
    m_tableWidget->setHorizontalHeaderLabels({"#", "Token Value", "Token Type", "Category & Details"});
    m_tableWidget->setAlternatingRowColors(true);
    m_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableWidget->verticalHeader()->setVisible(false);

    // Column resizing
    m_tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    m_tableWidget->setColumnWidth(0, 50);
    m_tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive);
    m_tableWidget->setColumnWidth(1, 145);
    m_tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Interactive);
    m_tableWidget->setColumnWidth(2, 135);
    m_tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);

    connect(m_tableWidget, &QTableWidget::cellDoubleClicked, this, &MainWindow::onTableItemDoubleClicked);
    layout->addWidget(m_tableWidget);

    // Diagnostics Alert Ribbon
    m_diagnosticsRibbon = new QWidget(rightContainer);
    m_diagnosticsRibbon->setFixedHeight(46);
    m_diagnosticsRibbon->setStyleSheet("background-color: #1A1A1D; border-top: 1px solid #2E2E32;");
    auto *diagLayout = new QHBoxLayout(m_diagnosticsRibbon);
    diagLayout->setContentsMargins(14, 6, 14, 6);
    diagLayout->setSpacing(10);

    m_diagWarningIcon = new QLabel("✓", m_diagnosticsRibbon);
    m_diagWarningIcon->setStyleSheet("color: #61DAC1; font-weight: bold; font-size: 15px;");
    diagLayout->addWidget(m_diagWarningIcon);

    m_diagMessageLabel = new QLabel("Ready. Press 'Scan Code' or F5 to analyze source code.", m_diagnosticsRibbon);
    m_diagMessageLabel->setStyleSheet("color: #E5E2E1; font-size: 12px;");
    diagLayout->addWidget(m_diagMessageLabel);

    m_diagCodeBadge = new QLabel("", m_diagnosticsRibbon);
    m_diagCodeBadge->setStyleSheet("background-color: #2A2A2E; color: #8A919D; border: 1px solid #38383E; "
                                   "border-radius: 4px; padding: 2px 7px; font-family: monospace; font-size: 10px;");
    m_diagCodeBadge->setVisible(false);
    diagLayout->addWidget(m_diagCodeBadge);

    diagLayout->addStretch();

    m_errorCounterLabel = new QLabel("", m_diagnosticsRibbon);
    m_errorCounterLabel->setStyleSheet("color: #8A919D; font-size: 11px; font-weight: 500;");
    m_errorCounterLabel->setVisible(false);
    diagLayout->addWidget(m_errorCounterLabel);

    m_prevErrorBtn = new QPushButton("◀", m_diagnosticsRibbon);
    m_prevErrorBtn->setFixedSize(28, 28);
    m_prevErrorBtn->setToolTip("Previous Lexical Error");
    m_prevErrorBtn->setVisible(false);
    connect(m_prevErrorBtn, &QPushButton::clicked, this, &MainWindow::onPrevErrorClicked);
    diagLayout->addWidget(m_prevErrorBtn);

    m_nextErrorBtn = new QPushButton("▶", m_diagnosticsRibbon);
    m_nextErrorBtn->setFixedSize(28, 28);
    m_nextErrorBtn->setToolTip("Next Lexical Error");
    m_nextErrorBtn->setVisible(false);
    connect(m_nextErrorBtn, &QPushButton::clicked, this, &MainWindow::onNextErrorClicked);
    diagLayout->addWidget(m_nextErrorBtn);

    m_goToLineBtn = new QPushButton("Go to line →", m_diagnosticsRibbon);
    m_goToLineBtn->setFixedHeight(28);
    m_goToLineBtn->setVisible(false);
    connect(m_goToLineBtn, &QPushButton::clicked, this, &MainWindow::onGoToErrorLineClicked);
    diagLayout->addWidget(m_goToLineBtn);

    layout->addWidget(m_diagnosticsRibbon);

    return rightContainer;
}

void MainWindow::setupStatusBar() {
    QStatusBar *bar = statusBar();

    m_statusLeftLabel = new QLabel("Status: Ready  |  TINY Lexer v1.0.0  |  UTF-8", this);
    bar->addWidget(m_statusLeftLabel, 1);

    m_statusCursorLabel = new QLabel("Ln 1, Col 1", this);
    m_statusTokensLabel = new QLabel("Tokens: 0", this);
    m_statusErrorsLabel = new QLabel("Errors: 0", this);

    bar->addPermanentWidget(m_statusCursorLabel);
    bar->addPermanentWidget(new QLabel("•", this));
    bar->addPermanentWidget(m_statusTokensLabel);
    bar->addPermanentWidget(new QLabel("•", this));
    bar->addPermanentWidget(m_statusErrorsLabel);
    bar->addPermanentWidget(new QLabel("  ", this));
}

void MainWindow::onOpenFile() {
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Open TINY Source File",
        m_currentFilePath.isEmpty() ? QDir::currentPath() : m_currentFilePath,
        "TINY Source Files (*.tny *.txt);;All Files (*.*)"
    );

    if (fileName.isEmpty()) return;

    QString content;
    QString error;
    if (m_controller.loadFile(fileName, content, error)) {
        m_currentFilePath = fileName;
        m_editor->setPlainText(content);
        QFileInfo info(fileName);
        m_fileTabLabel->setText(info.fileName() + "  •  " + fileName);

        // Clear previous token results; user must click 'Scan Code' to tokenize
        m_currentTokens.clear();
        m_errorTokenIndices.clear();
        m_currentErrorIndex = -1;
        m_editor->clearErrorHighlight();
        updateTokenTable();
        updateDiagnosticsRibbon();

        m_statusLeftLabel->setText(QString("File loaded: %1. Click 'Scan Code' to tokenize.").arg(info.fileName()));
        updateStatusBarMetrics();
    } else {
        QMessageBox::critical(this, "File Open Error", "Failed to open file:\n" + error);
    }
}

void MainWindow::onScanCode() {
    QString source = m_editor->toPlainText();
    m_editor->clearErrorHighlight();

    int errorCount = 0;
    double elapsedMs = 0.0;
    m_controller.scanSource(source, m_currentTokens, errorCount, elapsedMs);

    // Gather error token indices
    m_errorTokenIndices.clear();
    for (size_t i = 0; i < m_currentTokens.size(); ++i) {
        if (m_currentTokens[i].isError()) {
            m_errorTokenIndices.push_back(i);
        }
    }

    m_currentErrorIndex = m_errorTokenIndices.empty() ? -1 : 0;

    updateTokenTable();
    updateDiagnosticsRibbon();
    updateStatusBarMetrics(elapsedMs);
}

void MainWindow::onClearAll() {
    m_editor->clear();
    m_currentTokens.clear();
    m_errorTokenIndices.clear();
    m_currentErrorIndex = -1;
    m_currentFilePath.clear();
    m_fileTabLabel->setText("untitled.tny  •  Source Code (.tny / .txt)");

    updateTokenTable();
    updateDiagnosticsRibbon();
    updateStatusBarMetrics();
    m_statusLeftLabel->setText("Cleared all content and analysis results. Ready.");
}

void MainWindow::onExportTokens() {
    if (m_currentTokens.empty()) {
        QMessageBox::information(this, "Export Tokens", "No tokens available to export. Run 'Scan Code' first.");
        return;
    }

    QString defaultName = "tokens_output.txt";
    if (!m_currentFilePath.isEmpty()) {
        QFileInfo info(m_currentFilePath);
        defaultName = info.baseName() + "_tokens.txt";
    }

    QString savePath = QFileDialog::getSaveFileName(
        this,
        "Export Tokens to File",
        defaultName,
        "Text Files (*.txt);;All Files (*.*)"
    );

    if (savePath.isEmpty()) return;

    QString error;
    if (m_controller.exportTokensToFile(savePath, m_currentTokens, error)) {
        QMessageBox::information(
            this,
            "Export Successful",
            QString("Successfully exported %1 tokens to:\n%2").arg(m_currentTokens.size()).arg(savePath)
        );
    } else {
        QMessageBox::critical(this, "Export Failed", "Failed to export tokens:\n" + error);
    }
}

void MainWindow::updateTokenTable() {
    m_tableWidget->setRowCount(0);
    m_tableWidget->setSortingEnabled(false);

    int row = 0;
    for (const auto &token : m_currentTokens) {
        m_tableWidget->insertRow(row);

        // Col 0: #
        auto *itemIndex = new QTableWidgetItem(QString::number(row + 1));
        itemIndex->setTextAlignment(Qt::AlignCenter);
        itemIndex->setForeground(QColor("#73737C"));
        m_tableWidget->setItem(row, 0, itemIndex);

        // Col 1: Token Value
        auto *itemVal = new QTableWidgetItem(QString::fromStdString(token.value));
        itemVal->setFont(QFont("JetBrains Mono", 10));
        itemVal->setForeground(Theme::getTokenColor(token.type));
        m_tableWidget->setItem(row, 1, itemVal);

        // Col 2: Token Type (Badge Label)
        auto *itemType = new QTableWidgetItem();
        auto *badgeLabel = new QLabel(m_tableWidget);
        badgeLabel->setTextFormat(Qt::RichText);
        badgeLabel->setText(Theme::getTokenBadgeHtml(token.type));
        badgeLabel->setAlignment(Qt::AlignCenter);
        badgeLabel->setStyleSheet("background: transparent;");
        m_tableWidget->setItem(row, 2, itemType);
        m_tableWidget->setCellWidget(row, 2, badgeLabel);

        // Col 3: Details
        std::string desc = tokenDescription(token.type, token.value);
        if (token.isError() && !token.errorMessage.empty()) {
            desc = token.errorMessage + QString(" (line %1, col %2)").arg(token.line).arg(token.column).toStdString();
        }
        auto *itemDesc = new QTableWidgetItem(QString::fromStdString(desc));
        itemDesc->setForeground(token.isError() ? QColor("#FFB4AB") : QColor("#B0B8C4"));
        m_tableWidget->setItem(row, 3, itemDesc);

        row++;
    }

    m_tokenCountBadge->setText(QString("%1 Tokens Found").arg(m_currentTokens.size()));
    onFilterTextChanged(m_filterEdit->text());
}

void MainWindow::onFilterTextChanged(const QString &text) {
    QString filter = text.trimmed().toLower();
    for (int r = 0; r < m_tableWidget->rowCount(); ++r) {
        if (filter.isEmpty()) {
            m_tableWidget->setRowHidden(r, false);
            continue;
        }

        QString val = m_tableWidget->item(r, 1) ? m_tableWidget->item(r, 1)->text().toLower() : "";
        QString desc = m_tableWidget->item(r, 3) ? m_tableWidget->item(r, 3)->text().toLower() : "";

        QString typeStr = "";
        if (r < static_cast<int>(m_currentTokens.size())) {
            typeStr = QString::fromStdString(tokenTypeToString(m_currentTokens[r].type)).toLower();
        }

        bool match = val.contains(filter) || desc.contains(filter) || typeStr.contains(filter);
        m_tableWidget->setRowHidden(r, !match);
    }
}

void MainWindow::updateDiagnosticsRibbon() {
    int errorCount = static_cast<int>(m_errorTokenIndices.size());

    if (errorCount == 0) {
        if (m_currentTokens.empty()) {
            m_diagnosticsRibbon->setStyleSheet("background-color: #1A1A1D; border-top: 1px solid #2E2E32;");
            m_diagWarningIcon->setText("ℹ");
            m_diagWarningIcon->setStyleSheet("color: #8A919D; font-weight: bold; font-size: 14px;");
            m_diagMessageLabel->setText("Ready. Paste code or open a file, then click 'Scan Code' (F5).");
            m_diagMessageLabel->setStyleSheet("color: #8A919D; font-size: 12px;");
        } else {
            m_diagnosticsRibbon->setStyleSheet("background-color: #14241E; border-top: 1px solid #005144;");
            m_diagWarningIcon->setText("✓");
            m_diagWarningIcon->setStyleSheet("color: #61DAC1; font-weight: bold; font-size: 15px;");
            m_diagMessageLabel->setText("All tokens scanned cleanly. 0 lexical errors.");
            m_diagMessageLabel->setStyleSheet("color: #61DAC1; font-weight: 500; font-size: 12px;");
        }
        m_diagCodeBadge->setVisible(false);
        m_goToLineBtn->setVisible(false);
        m_prevErrorBtn->setVisible(false);
        m_nextErrorBtn->setVisible(false);
        m_errorCounterLabel->setVisible(false);
    } else {
        m_diagnosticsRibbon->setStyleSheet("background-color: #2D1416; border-top: 1px solid #93000A;");
        m_diagWarningIcon->setText("⚠");
        m_diagWarningIcon->setStyleSheet("color: #FFB4AB; font-weight: bold; font-size: 15px;");

        if (m_currentErrorIndex >= 0 && m_currentErrorIndex < errorCount) {
            size_t tokenIdx = m_errorTokenIndices[m_currentErrorIndex];
            const auto &errToken = m_currentTokens[tokenIdx];

            QString msg = QString("Warning: %1 found at line %2, column %3.")
                          .arg(QString::fromStdString(errToken.errorMessage))
                          .arg(errToken.line)
                          .arg(errToken.column);
            m_diagMessageLabel->setText(msg);
            m_diagMessageLabel->setStyleSheet("color: #FFB4AB; font-weight: 500; font-size: 12px;");

            m_diagCodeBadge->setText(QString("LEX_ERR_%1").arg(m_currentErrorIndex + 1, 3, 10, QChar('0')));
            m_diagCodeBadge->setVisible(true);

            m_errorCounterLabel->setText(QString("%1 of %2 errors").arg(m_currentErrorIndex + 1).arg(errorCount));
            m_errorCounterLabel->setVisible(true);

            m_prevErrorBtn->setVisible(errorCount > 1);
            m_nextErrorBtn->setVisible(errorCount > 1);
            m_goToLineBtn->setText(QString("Go to line %1 →").arg(errToken.line));
            m_goToLineBtn->setVisible(true);
        }
    }
}

void MainWindow::onGoToErrorLineClicked() {
    if (m_currentErrorIndex < 0 || m_currentErrorIndex >= static_cast<int>(m_errorTokenIndices.size())) {
        return;
    }

    size_t tokenIdx = m_errorTokenIndices[m_currentErrorIndex];
    const auto &errToken = m_currentTokens[tokenIdx];
    m_editor->highlightErrorLine(errToken.line, errToken.column);
    m_editor->setFocus();
}

void MainWindow::onNextErrorClicked() {
    if (m_errorTokenIndices.empty()) return;
    m_currentErrorIndex = (m_currentErrorIndex + 1) % m_errorTokenIndices.size();
    updateDiagnosticsRibbon();
    onGoToErrorLineClicked();
}

void MainWindow::onPrevErrorClicked() {
    if (m_errorTokenIndices.empty()) return;
    m_currentErrorIndex = (m_currentErrorIndex - 1 + m_errorTokenIndices.size()) % m_errorTokenIndices.size();
    updateDiagnosticsRibbon();
    onGoToErrorLineClicked();
}

void MainWindow::onTableItemDoubleClicked(int row, int /* column */) {
    if (row < 0 || row >= static_cast<int>(m_currentTokens.size())) return;
    const auto &tok = m_currentTokens[row];
    m_editor->highlightErrorLine(tok.line, tok.column);
    m_editor->setFocus();
}

void MainWindow::onCursorInfoChanged(int line, int col) {
    m_statusCursorLabel->setText(QString("Ln %1, Col %2").arg(line).arg(col));
}

void MainWindow::onEditorStatsChanged(int lines, int chars) {
    m_editorStatsLabel->setText(QString("%1 lines • %2 chars").arg(lines).arg(chars));
}

void MainWindow::updateStatusBarMetrics(double elapsedMs) {
    int tokenCount = static_cast<int>(m_currentTokens.size());
    int errorCount = static_cast<int>(m_errorTokenIndices.size());

    m_statusTokensLabel->setText(QString("Tokens: %1").arg(tokenCount));
    m_statusErrorsLabel->setText(QString("Errors: %1").arg(errorCount));

    if (elapsedMs > 0.0) {
        m_statusLeftLabel->setText(
            QString("Status: Scan Completed Successfully (%1 ms)  |  TINY Lexer v1.0.0  |  Ready")
            .arg(elapsedMs, 0, 'f', 2)
        );
    }
}

void MainWindow::onAbout() {
    QMessageBox::about(
        this,
        "About TINY Language Lexical Analyzer Studio",
        "<h3>TINY Language Lexical Analyzer Studio</h3>"
        "<p><b>Version:</b> 1.0.0</p>"
        "<p><b>Architecture:</b> Model-View-Controller (MVC) in Modern C++ and Qt Widgets.</p>"
        "<p><b>Compiler Front-End:</b> Deterministic Finite State Machine (DFA) based on Louden's TINY language specification.</p>"
        "<p><b>Supported Inputs:</b></p>"
        "<ul>"
        "<li>Source file loading via native <code>QFileDialog</code> (.tny, .txt)</li>"
        "<li>Direct code authoring or copy-pasting in the left editor pane</li>"
        "</ul>"
    );
}

void MainWindow::onTokenGrammarHelp() {
    QMessageBox::information(
        this,
        "TINY Language Token Grammar",
        "<b>Recognized TINY Language Tokens:</b><br><br>"
        "<b>Keywords:</b> if, then, else, end, repeat, until, read, write<br>"
        "<b>Special Symbols & Operators:</b><br>"
        "• := (ASSIGN)<br>"
        "• = (EQUAL), < (LESSTHAN)<br>"
        "• + (PLUS), - (MINUS), * (MULT), / (DIV)<br>"
        "• ; (SEMICOLON)<br>"
        "• ( (OPENBRACKET), ) (CLOSEDBRACKET)<br><br>"
        "<b>Identifiers:</b> Sequence of letters and alphanumerics (e.g., x, abc, count)<br>"
        "<b>Numbers:</b> One or more digits (e.g., 0, 12, 289)<br>"
        "<b>Comments:</b> Enclosed in curly braces { comment } or C-style //"
    );
}
