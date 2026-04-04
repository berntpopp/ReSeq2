# Contributing

Contributions to ReSeq2 are welcome. This page covers the development workflow, coding standards, and CI pipeline.

## Development Setup

1. Clone and build the project:

    ```bash
    git clone https://github.com/berntpopp/ReSeq2.git
    cd ReSeq2
    make build
    ```

2. Install pre-commit hooks:

    ```bash
    pip install pre-commit
    pre-commit install
    ```

3. Run the test suite to verify your setup:

    ```bash
    make test
    ```

## Code Style

### C++

- Formatter: **clang-format** (LLVM-based config in `.clang-format`)
- Standard: C++20
- Line length: 120 columns
- Indentation: 4 spaces (no tabs)
- Scope: `reseq/` directory only

Format your code before committing:

```bash
make format
```

Check without modifying files:

```bash
make format-check
```

### Python

- Formatter and linter: **ruff**
- Target: Python 3.9+
- Line length: 120 characters
- Rules: E, F, W, I, B, SIM
- Scope: `python/` directory only

```bash
make python-format       # Format in-place
make python-format-check # Check only
make python-lint         # Lint with ruff
```

### Linting

Static analysis with clang-tidy:

```bash
make lint
```

This runs clang-tidy on all C++ sources and ruff on Python sources.

## Commit Conventions

ReSeq2 uses [Conventional Commits](https://www.conventionalcommits.org/):

| Prefix | Use for |
|---|---|
| `feat:` | New features or commands |
| `fix:` | Bug fixes |
| `build:` | Build system or dependency changes |
| `style:` | Code formatting (no logic change) |
| `test:` | Adding or updating tests |
| `ci:` | CI/CD pipeline changes |
| `docs:` | Documentation changes |
| `refactor:` | Code restructuring (no behavior change) |
| `chore:` | Maintenance tasks |

Examples:

```
feat: add convertProfile command for format conversion
fix: handle empty quality string in ErrorStats
build: upgrade SeqAn to 2.5.2 via FetchContent
test: add golden-file regression tests for illuminaPE
```

The changelog is generated automatically from commit messages using git-cliff (`make changelog`).

## Adding Tests

Test files follow a consistent pattern:

1. Create `reseq/YourComponentTest.h` and `reseq/YourComponentTest.cpp`.

2. Inherit from `BasicTestClass`:

    ```cpp
    #include "BasicTestClass.hpp"
    #include "YourComponent.h"

    class YourComponentTest : public BasicTestClass {
      protected:
        // Test fixtures go here
    };
    ```

3. Write tests using GoogleTest macros:

    ```cpp
    TEST_F(YourComponentTest, HandlesEdgeCase) {
        YourComponent comp;
        EXPECT_EQ(comp.process(""), 0);
    }
    ```

4. Add the `.cpp` file to the test sources in `CMakeLists.txt` (the `reseq_test` target).

5. Run your new test:

    ```bash
    make test
    # Or filter to just your test:
    build/bin/reseq_test --gtest_filter="YourComponentTest.*"
    ```

!!! tip "Test data"
    Place test data files in the `test/` directory. For large files that should not be committed, add them to the `test/download_test_data.sh` script.

## CI Pipeline

GitHub Actions runs on every push and pull request (`.github/workflows/ci.yml`):

| Step | Description |
|---|---|
| **Multi-compiler build** | Builds with GCC 13 and Clang 17 on Ubuntu 24.04 |
| **Format check** | Verifies C++ and Python formatting |
| **Version check** | Ensures version tags match the `VERSION` file |
| **Test suite** | Runs the full CTest suite |
| **Sanitizers** | Runs ASan (AddressSanitizer) and UBSan (UndefinedBehaviorSanitizer) |
| **Coverage** | Generates code coverage reports |

!!! info "All checks must pass"
    Pull requests require all CI checks to pass before merging. Run `make format-check` and `make test` locally before pushing to catch issues early.

## Pull Request Workflow

1. Create a feature branch from `master`:

    ```bash
    git checkout -b feature/your-feature master
    ```

2. Make your changes, following the code style and commit conventions.

3. Ensure all checks pass locally:

    ```bash
    make format-check
    make test
    pre-commit run --all-files
    ```

4. Push and open a pull request against `master` on `berntpopp/ReSeq2`.

5. Address any CI failures or review feedback.
