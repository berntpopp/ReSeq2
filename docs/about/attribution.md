# Attribution

## Original ReSeq

ReSeq was created by **Stephan Schmeing** and **Mark D. Robinson** at the University of Zurich.

- **Paper:** Schmeing, S., Robinson, M.D. ReSeq simulates realistic Illumina high-throughput sequencing data. *Genome Biology* 22, 163 (2021). [DOI: 10.1186/s13059-021-02265-7](https://doi.org/10.1186/s13059-021-02265-7)
- **Original repository:** [github.com/schmeing/ReSeq](https://github.com/schmeing/ReSeq)

## What Changed in ReSeq2

ReSeq2 is a maintained continuation of the original project. The core simulation algorithms remain the same; the surrounding infrastructure has been modernized:

- **C++20 migration** --- Updated language standard with modern concurrency (`std::jthread`, `std::scoped_lock`), smart pointers, and `std::format`-ready code
- **Modernized CMake build** --- CMake 3.16+ with external dependency management via `FetchContent` and `find_package()` fallback, replacing vendored copies
- **CI/CD pipeline** --- GitHub Actions with multi-compiler builds (GCC 13 + Clang 17), ASan/UBSan sanitizers, and automated code coverage
- **Expanded test suite** --- Golden-file regression tests, component-level unit tests, and integration coverage
- **Binary/text profile conversion** --- New `convertProfile` command for portable profile sharing across architectures
- **Code quality tooling** --- clang-format, clang-tidy, ruff, and pre-commit hooks enforcing consistent style
- **Python plotting tools** --- Restructured as an installable Python package with type checking and unit tests

## Included Libraries

ReSeq2 includes or depends on the following open-source libraries:

| Library | License | Inclusion |
|---|---|---|
| [SeqAn 2.5.2](https://www.seqan.de/) | BSD 3-Clause | `FetchContent` / `find_package()` |
| [GoogleTest](https://github.com/google/googletest) | BSD 3-Clause | `FetchContent` |
| [NLopt](https://github.com/stevengj/nlopt) | MIT | `FetchContent` |
| [skewer](https://github.com/relipmoc/skewer) | MIT | Vendored with local modifications (see `skewer/MODIFICATIONS.md`) |

## Acknowledgments

ReSeq2 builds on the excellent scientific work of Stephan Schmeing and Mark D. Robinson. Their original implementation of the multi-dimensional probability estimation and realistic read simulation remains the foundation of this tool.
