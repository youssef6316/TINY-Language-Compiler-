#pragma once

#include <QPlainTextEdit>
#include <QWidget>

class LineNumberArea;

/**
 * @brief High-performance monospaced source code editor with line number gutter,
 * syntax layout, error line navigation, and cursor diagnostics.
 */
class CodeEditor : public QPlainTextEdit {
    Q_OBJECT

public:
    explicit CodeEditor(QWidget *parent = nullptr);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

    /**
     * @brief Navigates to a specific 1-based line, selects/highlights it, and scrolls into view.
     */
    void highlightErrorLine(int line, int col = 1);

    /**
     * @brief Clears any temporary error line highlighting.
     */
    void clearErrorHighlight();

signals:
    void cursorInfoChanged(int line, int column);
    void statsChanged(int totalLines, int totalChars);

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rect, int dy);
    void onCursorPositionChanged();
    void onTextChanged();

private:
    QWidget *m_lineNumberArea{nullptr};
    int m_errorHighlightedLine{-1};
};

/**
 * @brief Gutter widget for painting line numbers beside the editor.
 */
class LineNumberArea : public QWidget {
public:
    explicit LineNumberArea(CodeEditor *editor) : QWidget(editor), m_codeEditor(editor) {}

    QSize sizeHint() const override {
        return QSize(m_codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        m_codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    CodeEditor *m_codeEditor;
};
