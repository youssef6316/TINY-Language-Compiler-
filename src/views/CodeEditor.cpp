#include "CodeEditor.h"
#include <QPainter>
#include <QTextBlock>
#include <QFont>
#include <QTextCursor>

CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent) {
    m_lineNumberArea = new LineNumberArea(this);

    // Monospaced typography
    QFont font;
    font.setFamily("JetBrains Mono");
    if (!font.exactMatch()) {
        font.setFamily("Consolas");
    }
    font.setPointSize(11);
    font.setStyleHint(QFont::Monospace);
    setFont(font);

    // Tab size (4 spaces)
    setTabStopDistance(fontMetrics().horizontalAdvance(' ') * 4);

    connect(this, &CodeEditor::blockCountChanged, this, &CodeEditor::updateLineNumberAreaWidth);
    connect(this, &CodeEditor::updateRequest, this, &CodeEditor::updateLineNumberArea);
    connect(this, &CodeEditor::cursorPositionChanged, this, &CodeEditor::highlightCurrentLine);
    connect(this, &CodeEditor::cursorPositionChanged, this, &CodeEditor::onCursorPositionChanged);
    connect(this, &CodeEditor::textChanged, this, &CodeEditor::onTextChanged);

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}

int CodeEditor::lineNumberAreaWidth() {
    int digits = 1;
    int maxLines = qMax(1, blockCount());
    while (maxLines >= 10) {
        maxLines /= 10;
        digits++;
    }

    int space = 20 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * qMax(2, digits);
    return space;
}

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */) {
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy) {
    if (dy) {
        m_lineNumberArea->scroll(0, dy);
    } else {
        m_lineNumberArea->update(0, rect.y(), m_lineNumberArea->width(), rect.height());
    }

    if (rect.contains(viewport()->rect())) {
        updateLineNumberAreaWidth(0);
    }
}

void CodeEditor::resizeEvent(QResizeEvent *e) {
    QPlainTextEdit::resizeEvent(e);
    QRect cr = contentsRect();
    m_lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::onCursorPositionChanged() {
    QTextCursor cursor = textCursor();
    int line = cursor.blockNumber() + 1;
    int col = cursor.positionInBlock() + 1;
    emit cursorInfoChanged(line, col);
}

void CodeEditor::onTextChanged() {
    int lines = blockCount();
    int chars = toPlainText().length();
    emit statsChanged(lines, chars);
}

void CodeEditor::highlightCurrentLine() {
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;
        QColor lineColor = QColor("#1E1E22");
        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    // Retain error highlight if active
    if (m_errorHighlightedLine > 0 && m_errorHighlightedLine <= blockCount()) {
        QTextBlock block = document()->findBlockByNumber(m_errorHighlightedLine - 1);
        if (block.isValid()) {
            QTextEdit::ExtraSelection errSelection;
            errSelection.format.setBackground(QColor(147, 0, 10, 85));
            errSelection.format.setProperty(QTextFormat::FullWidthSelection, true);
            QTextCursor errCursor(block);
            errSelection.cursor = errCursor;
            extraSelections.append(errSelection);
        }
    }

    setExtraSelections(extraSelections);
}

void CodeEditor::highlightErrorLine(int line, int col) {
    if (line < 1 || line > blockCount()) return;

    m_errorHighlightedLine = line;

    QTextBlock block = document()->findBlockByNumber(line - 1);
    if (block.isValid()) {
        QTextCursor cursor(block);
        int targetCol = qMax(0, col - 1);
        cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, targetCol);
        setTextCursor(cursor);
        ensureCursorVisible();
    }

    highlightCurrentLine();
}

void CodeEditor::clearErrorHighlight() {
    m_errorHighlightedLine = -1;
    highlightCurrentLine();
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event) {
    QPainter painter(m_lineNumberArea);
    painter.fillRect(event->rect(), QColor("#18181A"));

    // Dividing border
    painter.setPen(QColor("#2E2E32"));
    painter.drawLine(m_lineNumberArea->width() - 1, event->rect().top(),
                     m_lineNumberArea->width() - 1, event->rect().bottom());

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());

    int currentLine = textCursor().blockNumber();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);

            if (blockNumber + 1 == m_errorHighlightedLine) {
                painter.setPen(QColor("#FFB4AB")); // Error line text
            } else if (blockNumber == currentLine) {
                painter.setPen(QColor("#FFFFFF")); // Active line text
            } else {
                painter.setPen(QColor("#73737C")); // Normal gutter text
            }

            painter.drawText(0, top, m_lineNumberArea->width() - 8, fontMetrics().height(),
                             Qt::AlignRight | Qt::AlignVCenter, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}

