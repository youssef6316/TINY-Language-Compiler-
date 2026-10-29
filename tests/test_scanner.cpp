#include "Scanner.h"
#include <iostream>
#include <cassert>
#include <vector>

void testBasicKeywordsAndSymbols() {
    std::string code = "read x;\nif x < 0 then\n  write 0;\nelse\n  write x;\nend;";
    Scanner scanner(code);
    auto tokens = scanner.tokenizeAll();

    // Verify token sequence
    std::vector<TokenType> expectedTypes = {
        TokenType::READ, TokenType::IDENTIFIER, TokenType::SEMICOLON,
        TokenType::IF, TokenType::IDENTIFIER, TokenType::LESSTHAN, TokenType::NUMBER, TokenType::THEN,
        TokenType::WRITE, TokenType::NUMBER, TokenType::SEMICOLON,
        TokenType::ELSE,
        TokenType::WRITE, TokenType::IDENTIFIER, TokenType::SEMICOLON,
        TokenType::END, TokenType::SEMICOLON
    };

    assert(tokens.size() == expectedTypes.size());
    for (size_t i = 0; i < tokens.size(); ++i) {
        assert(tokens[i].type == expectedTypes[i]);
    }
    std::cout << "[PASS] Basic keywords and symbols test passed (" << tokens.size() << " tokens)\n";
}

void testFactorialTinyProgram() {
    std::string code = 
        "{ Sample TINY program to compute factorial }\n"
        "read x; { input an integer }\n"
        "if 0 < x then { don't compute if x <= 0 }\n"
        "  fact := 1;\n"
        "  repeat\n"
        "    fact := fact * x;\n"
        "    x := x - 1\n"
        "  until x = 0;\n"
        "  write fact { output factorial of x }\n"
        "end\n";

    Scanner scanner(code);
    auto tokens = scanner.tokenizeAll();

    assert(scanner.getErrorCount() == 0);
    assert(tokens.size() > 20);

    // Check first token after comment is read at line 2
    assert(tokens[0].type == TokenType::READ);
    assert(tokens[0].line == 2);
    assert(tokens[0].value == "read");

    // Check assign ':=' operator
    bool foundAssign = false;
    for (const auto& t : tokens) {
        if (t.type == TokenType::ASSIGN) {
            foundAssign = true;
            assert(t.value == ":=");
        }
    }
    assert(foundAssign);

    std::cout << "[PASS] Factorial TINY program test passed (" << tokens.size() << " tokens)\n";
}

void testInvalidTokensAndErrorHandling() {
    std::string code = "x := 5 @ y := : 10 #";
    Scanner scanner(code);
    auto tokens = scanner.tokenizeAll();

    // '@', lone ':', '#' should be UNKNOWN
    int errors = 0;
    for (const auto& t : tokens) {
        if (t.isError()) {
            errors++;
            std::cout << "  Captured expected error: " << t.errorMessage 
                      << " at line " << t.line << ", col " << t.column 
                      << " ('" << t.value << "')\n";
        }
    }
    assert(errors >= 3);
    assert(scanner.getErrorCount() == errors);
    std::cout << "[PASS] Invalid tokens and error handling test passed (" << errors << " errors caught)\n";
}

void testExportFormat() {
    std::string code = "x := 4;";
    Scanner scanner(code);
    auto tokens = scanner.tokenizeAll();

    assert(tokens.size() == 4);
    assert(tokens[0].toExportString() == "x , IDENTIFIER");
    assert(tokens[1].toExportString() == ":= , ASSIGN");
    assert(tokens[2].toExportString() == "4 , NUMBER");
    assert(tokens[3].toExportString() == "; , SEMICOLON");

    std::cout << "[PASS] Export format test passed\n";
}

int main() {
    std::cout << "=== Running TINY Scanner Unit Tests ===\n";
    testBasicKeywordsAndSymbols();
    testFactorialTinyProgram();
    testInvalidTokensAndErrorHandling();
    testExportFormat();
    std::cout << "=== All Unit Tests Passed Successfully! ===\n";
    return 0;
}

