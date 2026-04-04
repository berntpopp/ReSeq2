# ReSeq2 Migration Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Complete the ReSeq2 v2.0.0 branding migration — version bump, binary/target rename, CLI branding, doc updates, release tag, and tracking issues.

**Architecture:** Edit `VERSION`, rename CMake targets from `reseq`→`reseq2` in three CMakeLists, update CLI strings in `main.cpp`, sed-replace CLI invocations across docs, tag v2.0.0, and file two GitHub tracking issues.

**Tech Stack:** CMake, C++, GitHub CLI (`gh`), Git

---

### Task 1: Bump Version to 2.0.0

**Files:**
- Modify: `VERSION`

- [ ] **Step 1: Update VERSION file**

Change the content from:
```
1.1.0
```
to:
```
2.0.0
```

- [ ] **Step 2: Verify CMake reads it**

Run:
```bash
cmake -S . -B build 2>&1 | head -5
```

Expected: No errors. Output should contain `reseq` project configured (project name changes in Task 2).

- [ ] **Step 3: Commit**

```bash
git add VERSION
git commit -m "build: bump version to 2.0.0 for ReSeq2 launch"
```

---

### Task 2: Rename CMake Targets and Binary

**Files:**
- Modify: `CMakeLists.txt:11`
- Modify: `reseq/CMakeLists.txt` (entire file — 18 occurrences)
- Modify: `python/CMakeLists.txt:70`
- Modify: `.github/workflows/ci.yml:182`

- [ ] **Step 1: Update root CMakeLists.txt**

Change line 11:
```cmake
project(reseq LANGUAGES CXX)
```
to:
```cmake
project(reseq2 LANGUAGES CXX)
```

- [ ] **Step 2: Update reseq/CMakeLists.txt — library target**

Change:
```cmake
add_library(reseq_lib STATIC
```
to:
```cmake
add_library(reseq2_lib STATIC
```

Change:
```cmake
target_include_directories(reseq_lib PUBLIC
```
to:
```cmake
target_include_directories(reseq2_lib PUBLIC
```

Change:
```cmake
target_link_libraries(reseq_lib PUBLIC
```
to:
```cmake
target_link_libraries(reseq2_lib PUBLIC
```

Change:
```cmake
target_compile_options(reseq_lib PRIVATE ${PRIVATE_COMPILE_OPTIONS})
```
(line 37) to:
```cmake
target_compile_options(reseq2_lib PRIVATE ${PRIVATE_COMPILE_OPTIONS})
```

- [ ] **Step 3: Update reseq/CMakeLists.txt — production executable**

Change:
```cmake
add_executable(reseq main.cpp)
target_sources(reseq PRIVATE cli/cli_common.cpp cli/convert_profile.cpp cli/illumina_pe.cpp cli/query_profile.cpp cli/replace_n.cpp cli/seq_to_illumina.cpp)
target_link_libraries(reseq PRIVATE reseq_lib)
target_compile_options(reseq PRIVATE ${PRIVATE_COMPILE_OPTIONS})

install(TARGETS reseq
```
to:
```cmake
add_executable(reseq2 main.cpp)
target_sources(reseq2 PRIVATE cli/cli_common.cpp cli/convert_profile.cpp cli/illumina_pe.cpp cli/query_profile.cpp cli/replace_n.cpp cli/seq_to_illumina.cpp)
target_link_libraries(reseq2 PRIVATE reseq2_lib)
target_compile_options(reseq2 PRIVATE ${PRIVATE_COMPILE_OPTIONS})

install(TARGETS reseq2
```

- [ ] **Step 4: Update reseq/CMakeLists.txt — test executable**

Change:
```cmake
  add_executable(reseq_test
```
to:
```cmake
  add_executable(reseq2_test
```

Change:
```cmake
  target_link_libraries(reseq_test PRIVATE reseq_lib GTest::gtest)
  target_compile_options(reseq_test PRIVATE ${PRIVATE_COMPILE_OPTIONS})
  # Run all tests as a single invocation — tests have inter-test dependencies
  # (shared Register() state) that require running together, not individually.
  add_test(NAME reseq_tests COMMAND reseq_test)
```
to:
```cmake
  target_link_libraries(reseq2_test PRIVATE reseq2_lib GTest::gtest)
  target_compile_options(reseq2_test PRIVATE ${PRIVATE_COMPILE_OPTIONS})
  # Run all tests as a single invocation — tests have inter-test dependencies
  # (shared Register() state) that require running together, not individually.
  add_test(NAME reseq2_tests COMMAND reseq2_test)
```

- [ ] **Step 5: Update reseq/CMakeLists.txt — coverage options**

Change:
```cmake
    target_compile_options(reseq_test PRIVATE --coverage -fprofile-update=atomic)
    target_link_options(reseq_test PRIVATE --coverage)
  endif()
endif()

if(CODE_COVERAGE)
  target_compile_options(reseq_lib PRIVATE --coverage -fprofile-update=atomic)
  target_link_options(reseq_lib PRIVATE --coverage)
  target_compile_options(reseq PRIVATE --coverage -fprofile-update=atomic)
  target_link_options(reseq PRIVATE --coverage)
endif()
```
to:
```cmake
    target_compile_options(reseq2_test PRIVATE --coverage -fprofile-update=atomic)
    target_link_options(reseq2_test PRIVATE --coverage)
  endif()
endif()

if(CODE_COVERAGE)
  target_compile_options(reseq2_lib PRIVATE --coverage -fprofile-update=atomic)
  target_link_options(reseq2_lib PRIVATE --coverage)
  target_compile_options(reseq2 PRIVATE --coverage -fprofile-update=atomic)
  target_link_options(reseq2 PRIVATE --coverage)
endif()
```

- [ ] **Step 6: Update python/CMakeLists.txt**

Change line 70:
```cmake
    reseq_lib
```
to:
```cmake
    reseq2_lib
```

- [ ] **Step 7: Update CI test-isolation job**

In `.github/workflows/ci.yml`, change line 182:
```yaml
        run: build/bin/reseq_test --gtest_shuffle --gtest_random_seed=0
```
to:
```yaml
        run: build/bin/reseq2_test --gtest_shuffle --gtest_random_seed=0
```

- [ ] **Step 8: Clean build directory and rebuild**

Run:
```bash
rm -rf build
cmake -S . -B build && cmake --build build -j$(nproc)
```

Expected: Clean build, binary at `build/bin/reseq2`, test binary at `build/bin/reseq2_test`.

- [ ] **Step 9: Run tests**

Run:
```bash
cd build && ctest --output-on-failure
```

Expected: All tests pass.

- [ ] **Step 10: Commit**

```bash
git add CMakeLists.txt reseq/CMakeLists.txt python/CMakeLists.txt .github/workflows/ci.yml
git commit -m "build: rename CMake targets and binary to reseq2"
```

---

### Task 3: Update CLI Branding in main.cpp

**Files:**
- Modify: `reseq/main.cpp:72-90`

- [ ] **Step 1: Update --version output**

Change line 72:
```cpp
        cerr << "ReSeq version " << RESEQ_VERSION_MAJOR << '.' << RESEQ_VERSION_MINOR << std::endl;
```
to:
```cpp
        cerr << "ReSeq2 version " << RESEQ_VERSION_MAJOR << '.' << RESEQ_VERSION_MINOR << '.' << RESEQ_VERSION_PATCH << std::endl;
```

- [ ] **Step 2: Update usage block**

Change lines 76-83:
```cpp
    string general_usage =
        string("\nProgram: reseq (REal SEQuence replicator)\n") + "Version: " + to_string(RESEQ_VERSION_MAJOR) + '.' +
        to_string(RESEQ_VERSION_MINOR) + '\n' + "Contact: Stephan Schmeing <stephan.schmeing@uzh.ch>\n\n" +
        "Usage:  reseq <command> [options]\n" + "Commands:\n" + "  illuminaPE\t\t" +
        "simulates illumina paired-end data\n" + "  queryProfile\t\t" +
        "queries reseq statistic files for information\n" + "  replaceN\t\t" + "replaces N's in reference\n" +
        "  seqToIllumina\t\t" + "applies illumina quality and error model to input sequences\n" + "  convertProfile\t" +
        "converts profiles between text and binary formats\n";
```
to:
```cpp
    string general_usage =
        string("\nProgram: reseq2 (REal SEQuence replicator 2)\n") + "Version: " + to_string(RESEQ_VERSION_MAJOR) +
        '.' + to_string(RESEQ_VERSION_MINOR) + '.' + to_string(RESEQ_VERSION_PATCH) + '\n' +
        "Contact: Bernt Popp (original: Stephan Schmeing <stephan.schmeing@uzh.ch>)\n\n" +
        "Usage:  reseq2 <command> [options]\n" + "Commands:\n" + "  illuminaPE\t\t" +
        "simulates illumina paired-end data\n" + "  queryProfile\t\t" +
        "queries reseq2 statistic files for information\n" + "  replaceN\t\t" + "replaces N's in reference\n" +
        "  seqToIllumina\t\t" + "applies illumina quality and error model to input sequences\n" +
        "  convertProfile\t" + "converts profiles between text and binary formats\n";
```

- [ ] **Step 3: Update running message**

Change lines 89-90:
```cpp
        printInfo << "Running ReSeq version " << RESEQ_VERSION_MAJOR << '.'
                  << RESEQ_VERSION_MINOR; // Always show version
```
to:
```cpp
        printInfo << "Running ReSeq2 version " << RESEQ_VERSION_MAJOR << '.' << RESEQ_VERSION_MINOR << '.'
                  << RESEQ_VERSION_PATCH; // Always show version
```

- [ ] **Step 4: Rebuild and verify**

Run:
```bash
cmake --build build -j$(nproc)
build/bin/reseq2 --version
build/bin/reseq2
```

Expected `--version` output:
```
ReSeq2 version 2.0.0
```

Expected usage output should show `Program: reseq2`, `Usage:  reseq2 <command>`, and updated contact.

- [ ] **Step 5: Commit**

```bash
git add reseq/main.cpp
git commit -m "feat: update CLI branding to ReSeq2"
```

---

### Task 4: Update Doc CLI References

**Files:**
- Modify: `README.md`
- Modify: `CLAUDE.md`
- Modify: `docs/getting-started/quickstart.md`
- Modify: `docs/user-guide/parameters.md`
- Modify: `docs/user-guide/commands.md`
- Modify: `docs/user-guide/error-model.md`
- Modify: `docs/user-guide/file-formats.md`
- Modify: `docs/user-guide/faq.md`
- Modify: `docs/user-guide/profiles.md`

- [ ] **Step 1: Update README.md**

Change:
```bash
reseq illuminaPE -j 32 -r reference.fa -b mappings.bam \
```
to:
```bash
reseq2 illuminaPE -j 32 -r reference.fa -b mappings.bam \
```

- [ ] **Step 2: Update docs/getting-started/quickstart.md**

Replace all CLI invocations. Every occurrence of `reseq illuminaPE` becomes `reseq2 illuminaPE`. There are ~7 occurrences in this file.

- [ ] **Step 3: Update docs/user-guide/parameters.md**

Replace all CLI synopsis lines:
- `reseq illuminaPE [options]` → `reseq2 illuminaPE [options]`
- `reseq queryProfile [options]` → `reseq2 queryProfile [options]`
- `reseq replaceN [options]` → `reseq2 replaceN [options]`
- `reseq seqToIllumina [options]` → `reseq2 seqToIllumina [options]`
- `reseq convertProfile [options]` → `reseq2 convertProfile [options]`

- [ ] **Step 4: Update docs/user-guide/commands.md**

Replace all CLI examples:
- `reseq illuminaPE` → `reseq2 illuminaPE`
- `reseq seqToIllumina` → `reseq2 seqToIllumina`
- `reseq queryProfile` → `reseq2 queryProfile`
- `reseq replaceN` → `reseq2 replaceN`
- `reseq convertProfile` → `reseq2 convertProfile`

- [ ] **Step 5: Update docs/user-guide/error-model.md**

Replace:
- `reseq seqToIllumina` → `reseq2 seqToIllumina`
- `reseq illuminaPE` → `reseq2 illuminaPE`
- `` `reseq replaceN` `` → `` `reseq2 replaceN` ``

- [ ] **Step 6: Update docs/user-guide/file-formats.md**

Replace:
- `reseq illuminaPE` → `reseq2 illuminaPE`

- [ ] **Step 7: Update docs/user-guide/faq.md**

Replace:
- `reseq queryProfile` → `reseq2 queryProfile`

- [ ] **Step 8: Update docs/user-guide/profiles.md**

Replace all CLI invocations:
- `reseq convertProfile` → `reseq2 convertProfile`
- `reseq illuminaPE` → `reseq2 illuminaPE`

- [ ] **Step 9: Update CLAUDE.md — binary and target names**

Replace in the Architecture section:
- `**\`reseq\`** — thin CLI executable` → `**\`reseq2\`** — thin CLI executable`
- `**\`reseq_test\`** — test executable` → `**\`reseq2_test\`** — test executable`
- `**\`reseq_lib\`**` → `**\`reseq2_lib\`**`

Replace in the Build and Test Commands section:
- `reseq_test` → `reseq2_test` in the GoogleTest filter example

Replace in the Architecture section:
- `One CMake static library + two executables:` — update the target descriptions to use `reseq2`, `reseq2_lib`, `reseq2_test`

- [ ] **Step 10: Verify no stale CLI references remain**

Run:
```bash
grep -rn '\breseq\b' README.md docs/ CLAUDE.md | grep -v '\.reseq' | grep -v 'schmeing' | grep -v 'reseq/' | grep -v 'reseq::' | grep -v 'python/reseq' | grep -v 'reseq2'
```

Expected: No output (all CLI references updated). Remaining `reseq` references should only be `.reseq` file extension, `schmeing/ReSeq` attribution, `reseq/` directory paths, or `reseq::` namespace.

- [ ] **Step 11: Commit**

```bash
git add README.md CLAUDE.md docs/
git commit -m "docs: update CLI references from reseq to reseq2"
```

---

### Task 5: Create Git Tag and GitHub Release

**Prerequisite:** Tasks 1-4 committed. Build and tests passing.

- [ ] **Step 1: Verify build is clean**

Run:
```bash
rm -rf build
cmake -S . -B build && cmake --build build -j$(nproc)
cd build && ctest --output-on-failure
```

Expected: All tests pass.

- [ ] **Step 2: Create annotated tag**

```bash
git tag -a v2.0.0 -m "$(cat <<'EOF'
ReSeq2 v2.0.0 - first independent release

Maintained continuation of ReSeq by Schmeing & Robinson.
Major changes from upstream v1.1:
- C++20 migration
- Modernized CMake build with external dependency management
- CI/CD with multi-compiler builds, sanitizers, coverage
- Expanded test suite
- Binary/text profile format conversion
- Code quality tooling (clang-format, clang-tidy, pre-commit)
- Full documentation site (mkdocs-material)
EOF
)"
```

- [ ] **Step 3: Push commits and tag**

```bash
git push origin master --tags
```

- [ ] **Step 4: Create GitHub release**

```bash
gh release create v2.0.0 \
  --repo berntpopp/ReSeq2 \
  --title "ReSeq2 v2.0.0" \
  --notes "$(cat <<'EOF'
## ReSeq2 v2.0.0

First independent release of ReSeq2, a maintained continuation of [ReSeq](https://github.com/schmeing/ReSeq) by Schmeing & Robinson ([Genome Biology, 2021](https://doi.org/10.1186/s13059-021-02265-7)).

### What's new compared to upstream ReSeq v1.1

- **C++20** — modernized language standard
- **CMake overhaul** — external dependency management via FetchContent (SeqAn 2.5.2, GoogleTest, NLopt)
- **CI/CD** — GitHub Actions with GCC 13 / Clang 17, ASan+UBSan+TSan, code coverage via Codecov
- **Test suite** — golden-file tests, edge-case tests, component-level tests
- **`convertProfile` command** — convert between binary and portable text profile formats
- **Code quality** — clang-format, clang-tidy, ruff, pre-commit hooks
- **Documentation** — full mkdocs-material site at [berntpopp.github.io/ReSeq2](https://berntpopp.github.io/ReSeq2/)

### Installation

```bash
git clone https://github.com/berntpopp/ReSeq2.git
cd ReSeq2
cmake -S . -B build && cmake --build build -j$(nproc)
```

Bioconda package (`reseq2`) coming soon.

### Attribution

ReSeq2 builds on the original work of Stephan Schmeing and Mark D. Robinson. Please cite the original paper when using ReSeq2.
EOF
)"
```

- [ ] **Step 5: Verify release**

```bash
gh release view v2.0.0 --repo berntpopp/ReSeq2
```

Expected: Release page shows with all notes.

---

### Task 6: Create Tracking Issues

- [ ] **Step 1: Create JOSS paper issue**

```bash
gh issue create \
  --repo berntpopp/ReSeq2 \
  --title "Write and submit JOSS paper for ReSeq2" \
  --label "documentation" \
  --body "$(cat <<'EOF'
## Summary

Write a JOSS (Journal of Open Source Software) paper for ReSeq2 (750-1750 words).

## Scope

- Describe ReSeq2 as a maintained continuation of ReSeq
- Highlight improvements: C++20, CMake modernization, CI/CD, expanded tests, convertProfile, code quality tooling
- Reference the original Genome Biology paper
- Target audience: bioinformatics community using Illumina sequencing simulation

## References

- JOSS submission guidelines: https://joss.readthedocs.io/
- Original paper: https://doi.org/10.1186/s13059-021-02265-7
EOF
)"
```

- [ ] **Step 2: Create Bioconda recipe issue**

```bash
gh issue create \
  --repo berntpopp/ReSeq2 \
  --title "Submit Bioconda recipe for reseq2" \
  --label "packaging" \
  --body "$(cat <<'EOF'
## Summary

Submit a Bioconda recipe for `reseq2` after the v2.0.0 release.

## Steps

1. Get the sha256 of the v2.0.0 source tarball from the GitHub release
2. Create `meta.yaml` following Bioconda conventions
3. Submit PR to https://github.com/bioconda/bioconda-recipes
4. Test with: `reseq2 --version`

## Key details

- Package name: `reseq2` (distinct from the unmaintained upstream `reseq` package)
- Build requirements: cmake >=3.16, C++20 compiler, boost, zlib, bzip2
- Linux only (skip macOS initially)

## Reference

See `.planning/2026-04-04-reseq2-repo-migration.md` Task 9 for a draft `meta.yaml` template.
EOF
)"
```

- [ ] **Step 3: Verify issues created**

```bash
gh issue list --repo berntpopp/ReSeq2 --limit 5
```

Expected: Both issues visible in the list.
