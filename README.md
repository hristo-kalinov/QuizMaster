# QuizMaster

A lightweight, object-oriented C++ console application for creating, managing, and taking quizzes. Built using core OOP principles and manual dynamic memory management (no STL containers or `std::string`) to showcase foundational C++ systems design.

---

## Features

- **Authentication System:** Binary file persistence (`users.dat`) for user registration, credential validation, and session tracking.
- **Polymorphic Quiz Engine:** Supports multiple question types (True/False, Single Choice) derived from an abstract base class.
- **Interactive Quiz Creation:** Wizard to author quizzes with customizable point values, saved in human-readable plain text (`quizzes.txt`).
- **XP Progression:** Tracks and accumulates player experience points as quizzes are completed.
- **Cross-Platform:** CMake build system compatible with Linux (GCC/Clang) and Windows (MSVC).

---

## OOP & Design Highlights

- **Polymorphism & Abstraction:** Abstract `Question` base class defining a unified interface (`ask()`, `checkAnswer()`, `clone()`, `saveToFile()`, `loadFromFile()`).
- **Prototype Pattern:** Implements a virtual `clone()` method for deep copying polymorphic question objects.
- **Rule of Three Compliance:** Custom copy constructors, copy assignment operators (`operator=`), and virtual destructors ensuring clean manual heap allocation (`char*` and dynamic pointer arrays) with zero memory leaks (verified via Valgrind).
- **Inheritance Hierarchy:** `User` base class extended by `Player` with clear separation of identity and gameplay logic.

---

## Build & Run

### Prerequisites
- C++11 compatible compiler (GCC, Clang, or MSVC)
- CMake 3.10+

### Compilation
```bash
cmake -B build -S .
cmake --build build
```

### Running the Application
```bash
cd build
./QuizMaster1
```

---

## Command Reference

| Command | Description |
| :--- | :--- |
| `help` | Display available commands |
| `signup <username> <password>` | Register a new user |
| `login <username> <password>` | Log into an existing account |
| `quizzes` | List all available quizzes *(login required)* |
| `start-quiz <quiz_id>` | Start and complete a quiz *(login required)* |
| `create-quiz` | Interactive quiz creator *(login required)* |
| `exit` | Quit the application |
