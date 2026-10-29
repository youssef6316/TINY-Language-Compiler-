#include "MainController.h"
#include <QFile>
#include <QTextStream>
#include <chrono>

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QStringConverter>
#endif

bool MainController::loadFile(const QString &filePath, QString &outContent, QString &outError) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        outError = file.errorString();
        return false;
    }

    QTextStream in(&file);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    in.setEncoding(QStringConverter::Utf8);
#else
    in.setCodec("UTF-8");
#endif
    outContent = in.readAll();
    file.close();
    return true;
}

bool MainController::exportTokensToFile(const QString &filePath, const std::vector<Token> &tokens, QString &outError) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        outError = file.errorString();
        return false;
    }

    QTextStream out(&file);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    out.setEncoding(QStringConverter::Utf8);
#else
    out.setCodec("UTF-8");
#endif

    // Export each token in the exact specification format: "<tokenvalue> , <tokentype>"
    for (const auto &token : tokens) {
        out << QString::fromStdString(token.toExportString()) << "\n";
    }

    file.close();
    return true;
}

void MainController::scanSource(const QString &sourceCode,
                                std::vector<Token> &outTokens,
                                int &outErrorCount,
                                double &outElapsedMs) {
    auto start = std::chrono::high_resolution_clock::now();

    Scanner scanner(sourceCode.toStdString());
    outTokens = scanner.tokenizeAll(/*includeEndfile=*/false);
    outErrorCount = scanner.getErrorCount();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    outElapsedMs = elapsed.count();
}

