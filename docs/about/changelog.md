# Changelog

## v1.1.0 (Current)

ReSeq2 --- maintained continuation of the original ReSeq with modernized infrastructure. The core simulation algorithms are unchanged; the surrounding build system, tests, and tooling have been overhauled across eight phases of work.

### Phase 1: Build System Modernization

- Migrated to CMake 3.16+ with modern target-based configuration
- Adopted C++20 as the language standard (GCC 10+, Clang 12+)
- External dependencies (SeqAn 2.5.2, GoogleTest, NLopt) managed via `FetchContent` with `find_package()` fallback
- Removed vendored copies of SeqAn and GoogleTest

### Phase 2: Golden-File Regression Tests

- Added golden-file tests that capture known-good output for the `illuminaPE` pipeline
- Established regression testing baseline to protect against behavioral drift during refactoring

### Phase 3: Safety Stabilization

- Converted raw pointers to smart pointers (`std::unique_ptr`, `std::shared_ptr`)
- Replaced `NULL` with `nullptr` throughout the codebase
- Migrated mutex usage to `std::scoped_lock` for exception-safe locking

### Phase 4: Concurrency Modernization

- Replaced manual thread management with `std::jthread` for automatic join-on-destruct
- Introduced atomic verbosity flag for thread-safe logging control

### Phase 5: God Object Decomposition

- Decomposed monolithic `main.cpp` into focused command modules
- Extracted `DataStats` sub-components into independently testable units

### Phase 6: DRY and Interface Cleanup

- Extracted magic numbers into named constants
- Introduced type aliases for common complex types
- Removed `FRIEND_TEST` macros in favor of proper test interfaces

### Phase 7: Test Coverage and CI

- Added clang-tidy static analysis to the build pipeline
- Enabled ASan (AddressSanitizer) and UBSan (UndefinedBehaviorSanitizer) in CI
- Added automated code coverage reporting
- GitHub Actions CI with GCC 13 and Clang 17 on Ubuntu 24.04

### Phase 8: Polish and Packaging

- C++20 cleanup: removed compatibility shims, adopted standard library features
- Extracted `types.hpp` from `utilities.hpp` for cleaner dependency structure
- Added `convertProfile` command for binary/text profile format conversion
- Restructured Python plotting tools as an installable package with type checking and tests
- Pre-commit hooks enforcing clang-format, ruff, and conventional commits

---

## v1.1 (Original)

Last upstream release by Stephan Schmeing ([schmeing/ReSeq](https://github.com/schmeing/ReSeq)).

- Full Illumina paired-end simulation pipeline
- Iterative Proportional Fitting for multi-dimensional probability estimation
- Support for adapter detection, GC bias, fragment length distributions
- `seqToIllumina`, `queryProfile`, and `replaceN` commands
