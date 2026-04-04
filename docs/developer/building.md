# Building

This page covers all build targets, testing, code coverage, and optional Python bindings.

## Prerequisites

See [Installation](../getting-started/installation.md) for the full list of required and optional system dependencies.

## Make Targets

The project provides a `Makefile` that wraps CMake for common workflows:

| Target | Description |
|---|---|
| `make build` | Configure (CMake) and compile all C++ targets |
| `make test` | Build, download test data if needed, and run the full test suite via CTest |
| `make coverage` | Build with coverage instrumentation, run tests, generate an HTML report |
| `make format` | Format C++ sources (clang-format) and Python code (ruff) in-place |
| `make format-check` | Dry-run format check --- exits non-zero on violations |
| `make lint` | Run clang-tidy on C++ and ruff on Python |
| `make clean` | Remove the entire build directory |
| `make changelog` | Generate `CHANGELOG.md` via git-cliff |
| `make install` | Build and install to the system (default `/usr/local`) |

!!! tip "Build configuration"
    You can customize the build via environment variables:

    ```bash
    BUILD_TYPE=Debug make build           # Debug build
    CMAKE_FLAGS="-DRESEQ_BUILD_PYTHON=ON" make build  # Enable Python bindings
    ```

## Running Tests

Tests compile into a separate `reseq_test` binary and run through CTest.

### Full test suite

```bash
make test
```

This automatically downloads any missing test data (E. coli and Drosophila references, BAM files) before running.

### Running tests directly

```bash
ctest --test-dir build --output-on-failure
```

### Filtering tests with GoogleTest

Use `--gtest_filter` to run a subset of tests:

```bash
# Run all Simulator tests
build/bin/reseq_test --gtest_filter="SimulatorTest.*"

# Run a single test
build/bin/reseq_test --gtest_filter="ReferenceTest.LoadsEcoliGenome"

# Run everything except slow tests
build/bin/reseq_test --gtest_filter="-*Slow*"
```

!!! warning "Sequential execution required"
    Tests must run sequentially (not in parallel) due to inter-test dependencies on shared test data. Do **not** pass `-j` to CTest.

## Code Coverage

Generate an HTML coverage report:

```bash
make coverage
```

This performs the following steps:

1. Configures a Debug build with `-DCODE_COVERAGE=ON`
2. Builds and runs the full test suite
3. Captures coverage data with `lcov`
4. Strips vendored (`skewer/`), system, and build-directory paths
5. Generates an HTML report at `build/coverage-report/index.html`

!!! note "Required tools"
    Coverage requires `lcov` and `genhtml`. On Ubuntu/Debian:

    ```bash
    sudo apt install lcov
    ```

## Pre-commit Hooks

The project uses [pre-commit](https://pre-commit.com/) to enforce formatting and linting on every commit.

### Setup

```bash
pip install pre-commit
pre-commit install
```

### Running manually

```bash
pre-commit run --all-files
```

Pre-commit hooks enforce formatting on `reseq/` (C++) and `python/` (Python) only. Other directories are excluded.

## Python Bindings

Python bindings are **off by default**. To enable them:

=== "Build with Make"

    ```bash
    CMAKE_FLAGS="-DRESEQ_BUILD_PYTHON=ON" make build
    ```

=== "Build with CMake directly"

    ```bash
    cmake -S . -B build -DRESEQ_BUILD_PYTHON=ON
    cmake --build build -j$(nproc)
    ```

Additional requirements for Python bindings:

| Dependency | Ubuntu / Debian |
|---|---|
| python3-dev | `sudo apt install python3-dev` |
| SWIG 3+ | `sudo apt install swig` |

After building, install the Python package:

```bash
pip install .
```

## Python Tools

ReSeq2 ships Python utilities for plotting and read-name preparation in `python/reseq/`. These have their own quality targets:

| Target | Description |
|---|---|
| `make python-format` | Format Python code with ruff |
| `make python-format-check` | Check Python formatting (dry-run) |
| `make python-lint` | Lint Python code with ruff |
| `make python-typecheck` | Type-check with mypy |
| `make python-test` | Run Python unit tests |
| `make python-verify` | Run all Python checks (format + lint + typecheck + test) |

## CMake Options

| Option | Default | Description |
|---|---|---|
| `CMAKE_BUILD_TYPE` | `RelWithDebInfo` | Build type (`Debug`, `Release`, `RelWithDebInfo`) |
| `DRESEQ_BUILD_PYTHON` | `OFF` | Build Python SWIG bindings |
| `DCODE_COVERAGE` | `OFF` | Enable code coverage instrumentation |
| `CMAKE_EXPORT_COMPILE_COMMANDS` | `ON` (via Make) | Generate `compile_commands.json` for clang-tidy |
