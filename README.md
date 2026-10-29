# TINY Language Lexical Analyzer Studio

[![Build Standalone Windows Executable](https://github.com/youssef-osama11/TINY-Language-Compiler/actions/workflows/build_windows.yml/badge.svg)](https://github.com/youssef-osama11/TINY-Language-Compiler/actions/workflows/build_windows.yml)
[![Language](https://img.shields.io/badge/Language-C%2B%2B17-00599C?logo=c%2B%2B)](https://en.cppreference.com/w/cpp/17)
[![Framework](https://img.shields.io/badge/GUI-Qt%206%20%2F%20Qt%205-41CD52?logo=qt)](https://www.qt.io/)
[![Platform](https://img.shields.io/badge/Platform-Windows%20(Standalone%20.exe)-0078D6?logo=windows)](https://github.com)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

A high-performance desktop lexical analyzer (scanner) application for the **TINY Programming Language**, engineered in **Modern C++ (C++17)** and **Qt Widgets**. Built in strict accordance with Kenneth C. Louden's compiler construction specifications and the course requirements, featuring a clean **Model-View-Controller (MVC)** architecture, a deterministic finite automaton (DFA) state machine, real-time error diagnostics, and automated standalone Windows executable packaging.

---

## Visual Design & User Interface

The application interface implements the **Developer Minimalism** design system specified in Google Stitch project **`TINY Lexical Analyzer Studio`**:

- **Dark Mode Ergonomics:** Built upon deep charcoal surfaces (`#131313`, `#1E1E1E`, `#202020`) with semantic accent highlights (`#007ACC` primary blue, `#61DAC1` identifier mint, `#F6BB84` literal amber, `#FFB4AB` error red).
- **Split-Pane Layout (`QSplitter`):** Dynamically resizable 50/50 split dividing the source code editor from the analysis grid.
- **Monospaced Code Editor (`CodeEditor`):** Custom line-number gutter, active line tracking, and automatic line-jump highlighting for compiler errors.
- **Structured Token Grid (`QTableWidget`):** Color-coded category chips, lexeme strings, token types, and syntactic descriptions.
- **Interactive Search Filter:** Real-time filtering across lexemes, categories, and token types.
- **Diagnostics Ribbon:** Visual warning bar with error codes (`LEX_ERR_001`), line and column indicators, and one-click **"Go to line"** navigation.

---

## Architecture Overview (MVC Pattern)

The codebase enforces a separation of concerns through the Model-View-Controller pattern:

```mermaid
flowchart LR
    subgraph View ["View Layer (Qt Widgets)"]
        MW["MainWindow"]
        CE["CodeEditor (Line Numbers)"]
        TW["QTableWidget (Token Stream)"]
        DR["Diagnostics Ribbon"]
    end

    subgraph Controller ["Controller Layer"]
        MC["MainController"]
    end

    subgraph Model ["Model Layer (Core Compiler)"]
        SC["Scanner (DFA State Machine)"]
        TK["Token / TokenType"]
    end

    MW -->|Triggers Scan / I/O| MC
    MC -->|Runs getToken / tokenizeAll| SC
    SC -->|Generates| TK
    MC -->|Delivers Tokens & Metrics| MW
    MW -->|Updates| TW
    MW -->|Renders Errors| DR
    CE -->|Syncs Line / Col| MW
```

### Component Breakdown
1. **Models Layer (`include/Token.h`, `src/Token.cpp`, `include/Scanner.h`, `src/Scanner.cpp`):**
   - Pure C++17 without GUI dependencies; can be compiled and verified independently.
   - `TokenType`: Strongly typed enumeration of all TINY keywords, operators, delimiters, and errors.
   - `Token`: Data structure encapsulating the lexeme string value, token type, line number, column number, and error descriptions.
   - `Scanner`: Implements the `getToken()` lexical state machine, whitespace stripping, comment removal (`{ ... }` and `// ...`), and maximal munch lookahead.
2. **Controllers Layer (`include/MainController.h`, `src/MainController.cpp`):**
   - Coordinates file streaming, token export formatting, benchmarking (`std::chrono`), and sample preloading.
3. **Views Layer (`include/MainWindow.h`, `src/MainWindow.cpp`, `include/CodeEditor.h`, `src/CodeEditor.cpp`, `include/Theme.h`, `src/Theme.cpp`):**
   - Implements the complete desktop GUI, menus, toolbars, line gutter painting, cell badges, and status bar telemetry.

---

## TINY Language Lexical Specification

### 1. Recognized Token Set
| Token Type | Value / Syntax Example | Category | Description |
| :--- | :--- | :--- | :--- |
| `SEMICOLON` | `;` | Delimiter | Statement Terminator |
| `IF` | `if` | Keyword | Conditional Branch Introducer |
| `THEN` | `then` | Keyword | Conditional Consequent Block |
| `ELSE` | `else` | Keyword | Conditional Alternative Block |
| `END` | `end` | Keyword | Block Terminus |
| `REPEAT` | `repeat` | Keyword | Iteration Loop Introducer |
| `UNTIL` | `until` | Keyword | Loop Termination Condition |
| `READ` | `read` | Keyword | Input Instruction |
| `WRITE` | `write` | Keyword | Output Instruction |
| `ASSIGN` | `:=` | Operator | Variable Assignment |
| `EQUAL` | `=` | Operator | Equality Comparison |
| `LESSTHAN` | `<` | Operator | Relational Less-Than Comparison |
| `PLUS` | `+` | Operator | Arithmetic Addition |
| `MINUS` | `-` | Operator | Arithmetic Subtraction |
| `MULT` | `*` | Operator | Arithmetic Multiplication |
| `DIV` | `/` | Operator | Arithmetic Division |
| `OPENBRACKET` | `(` | Delimiter | Left Grouping Parenthesis |
| `CLOSEDBRACKET` | `)` | Delimiter | Right Grouping Parenthesis |
| `IDENTIFIER` | `x`, `abc`, `fact`, `temp1` | Identifier | Sequence of letters and alphanumerics |
| `NUMBER` | `0`, `12`, `289` | Number | Sequence of decimal digits |
| `UNKNOWN` | `@`, `$`, `#`, lone `:`, etc. | Error | Unrecognized symbol or invalid sequence |

### 2. DFA State Machine
The lexical analyzer operates as a Deterministic Finite Automaton with the following states:
- `START`: Initial state scanning incoming characters.
- `INNUM`: Accumulates digits until a non-digit delimiter is encountered (lookahead character unread).
- `INID`: Accumulates alphanumeric characters until a non-alphanumeric character is encountered; checks reserved keywords table.
- `INASSIGN`: Encountered `:`; expects `=`. If `=`: returns `ASSIGN` (`:=`); otherwise reports `UNKNOWN` error.
- `INCOMMENT_CURLY`: Consumes characters inside curly braces `{ ... }` until `}` is reached.
- `INCOMMENT_LINE`: Consumes single-line comments (`// ...`) until newline or EOF.
- `DONE`: Final state returning the extracted `Token`.

---

## File I/O & Export Specifications

### 1. Open Source File
Click **"Open File"** or press `Ctrl+O` to load `.tny` or `.txt` source files directly into the left editor pane using native file dialogs.

### 2. Export Tokens
Click **"Export Tokens"** or press `Ctrl+S` to dump the generated token sequence into a text file. As mandated by the project specifications, the output format is:
```text
<tokenvalue> , <tokentype>
```

#### Example Output:
```text
read , READ
x , IDENTIFIER
; , SEMICOLON
if , IF
0 , NUMBER
< , LESSTHAN
x , IDENTIFIER
then , THEN
fact , IDENTIFIER
:= , ASSIGN
1 , NUMBER
; , SEMICOLON
```

---

## Automated Standalone Windows Executable CI/CD

This repository includes a production-grade GitHub Actions workflow (`.github/workflows/build_windows.yml`) that automatically:
1. Triggers on any `git push` or `pull_request`.
2. Sets up the MSVC compiler toolchain, CMake, and Ninja on a clean `windows-latest` runner.
3. Installs Qt 6.6 via `jurplel/install-qt-action`.
4. Compiles the project and runs the automated unit test suite (`ctest`).
5. Invokes `windeployqt` to collect all required Qt shared libraries (`Qt6Core.dll`, `Qt6Gui.dll`, `Qt6Widgets.dll`, `qwindows.dll` platform plugin, styles, etc.).
6. Packages the standalone executable into **`TINY-Lexical-Analyzer-Windows-x64.zip`**.
7. Publishes the zip archive as an artifact downloadable directly from GitHub without needing Qt or C++ development tools installed on your PC.

### How to Download the Standalone `.exe`:
1. Go to the **Actions** tab of this repository.
2. Click on the latest workflow run.
3. Scroll down to the **Artifacts** section.
4. Download **`TINY-Lexical-Analyzer-Windows-x64.zip`**, extract it, and run `TINYScanner.exe`.

---

## Local Build Instructions

### Prerequisites
- **Compiler:** MSVC 2019/2022, GCC 9+ (MinGW-w64), or Clang (C++17 support required).
- **Build System:** CMake 3.16 or later.
- **Framework:** Qt 6 (6.2+) or Qt 5 (5.15+).

### 1. Configure and Build (CLI)
```powershell
# Clone the repository
git clone https://github.com/youssef-osama11/TINY-Language-Compiler.git
cd TINY-Language-Compiler

# Create build directory
mkdir build
cd build

# Configure with CMake (specify Qt path if not in system PATH)
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build executable and unit tests
cmake --build . --config Release
```

### 2. Run the Unit Tests
```powershell
# Execute test suite
ctest --output-on-failure
# Or run standalone unit test binary:
./ScannerUnitTests
```

### 3. Run Standalone Deployment (`windeployqt`)
```powershell
# Deploy Qt dependencies next to the executable
windeployqt --release bin/TINYScanner.exe
```

---

## Included Sample Programs
Three preloaded sample programs are available under `tests/samples/` and directly via the application toolbar:
1. **`sample1_factorial.tny`:** The classic TINY factorial computation program demonstrating valid keywords, loops, conditions, and arithmetic operators.
2. **`sample2_invalid.tny`:** Demonstration of invalid characters (`@`, lone `:`, `$`, `#`) triggering the warning ribbon and line navigation.
3. **`sample3_fibonacci.tny`:** Fibonacci sequence calculation showcasing assignment, loops, and statement delimiters.

---

## Project Structure

```text
TINY-Language-Compiler/
├── .github/
│   └── workflows/
│       └── build_windows.yml      # Automated standalone .exe build & packaging
├── Docs/
│   └── ScannerProjectDescription.pdf # Course project specifications
├── include/
│   ├── Token.h                    # Token structure & TokenType enum
│   ├── Scanner.h                  # Core DFA lexical analyzer model
│   ├── MainController.h           # MVC Controller (I/O & metrics)
│   ├── MainWindow.h               # MVC View (Split-pane layout & dialogs)
│   ├── CodeEditor.h               # Custom code editor with line gutter
│   └── Theme.h                    # Stitch design system styling & badges
├── src/
│   ├── main.cpp                   # Application entry point
│   ├── Token.cpp                  # Token string conversions & export format
│   ├── Scanner.cpp                # DFA state machine implementation
│   ├── MainController.cpp         # File I/O & scan execution orchestration
│   ├── MainWindow.cpp             # GUI layout, slots, table & diagnostics
│   ├── CodeEditor.cpp             # Line number painting & error highlighting
│   └── Theme.cpp                  # QSS stylesheet & category badge generator
├── tests/
│   ├── test_scanner.cpp           # Automated C++ unit test suite
│   └── samples/
│       ├── sample1_factorial.tny  # Sample TINY program
│       ├── sample2_invalid.tny    # Sample program with lexical errors
│       └── sample3_fibonacci.tny  # Sample Fibonacci program
├── CMakeLists.txt                 # Modern CMake configuration
├── README.md                      # Comprehensive documentation
└── LICENSE                        # MIT License
```

---

## Academic & Engineering Credits
Developed for the **Compiler Design Course (Senior 2)**.
- **Specification:** Louden, Kenneth C. *Compiler Construction: Principles and Practice*.
- **Design System:** `TINY Lexical Analyzer Studio`.
- **Author:** Senior Engineering Team.
