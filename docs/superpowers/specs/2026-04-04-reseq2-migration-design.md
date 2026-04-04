# ReSeq2 Migration Design

**Goal:** Complete the ReSeq2 branding migration — bump to v2.0.0, rename build targets/binary to `reseq2`, update CLI branding, update all doc references, tag a release, and create tracking issues for deferred work.

**Repo:** `berntpopp/ReSeq2` (existing repo, no new repo creation)

---

## Context

The docs overhaul (PR #1) already completed: LICENSE dual copyright, README rebrand, CLAUDE.md update, full mkdocs site, and GitHub Pages deployment. A courtesy PR was filed on `schmeing/ReSeq`. What remains is the build/binary rename, version bump, CLI branding, and release.

## Decisions

| Decision | Choice | Rationale |
|----------|--------|-----------|
| Repo name | Keep `berntpopp/ReSeq2` | Already set up, docs/badges/Pages all reference it |
| Binary name | `reseq2` | Clean separation from upstream Bioconda `reseq` package |
| Source directory | Keep `reseq/` | Internal detail, renaming would be a massive diff for no user benefit |
| Python package dir | Keep `python/reseq/` | Same rationale |
| C++ namespace | Keep `reseq::` | Internal, no user-facing impact |
| Bioconda recipe | Deferred to post-release | Needs tarball sha256 |
| JOSS paper | Deferred, tracking issue | Independent effort |

## Out of Scope

- Creating a new GitHub repository (already exists)
- LICENSE changes (already done)
- README rewrite (already done, only CLI command references updated)
- CLAUDE.md rewrite (already done, only binary names updated)
- mkdocs site structure (already done, only CLI command references updated)
- Bioconda recipe (post-release)
- JOSS paper (separate effort)
- Source directory or namespace rename

---

## Task A: Version Bump

**Files:** `VERSION`

Change `1.1.0` to `2.0.0`. Verify CMake reads it (`cmake -S . -B build` should show no errors).

**Commit:** `build: bump version to 2.0.0 for ReSeq2 launch`

---

## Task B: CMake Target and Binary Rename

**Files:** `CMakeLists.txt`, `reseq/CMakeLists.txt`, `python/CMakeLists.txt`, `.github/workflows/ci.yml`

### Root CMakeLists.txt
- `project(reseq LANGUAGES CXX)` → `project(reseq2 LANGUAGES CXX)`

### reseq/CMakeLists.txt
All target names change:
- `reseq_lib` → `reseq2_lib` (static library)
- `reseq` → `reseq2` (CLI executable)
- `reseq_test` → `reseq2_test` (test executable)
- `reseq_tests` → `reseq2_tests` (CTest test name)

Every `target_link_libraries`, `target_include_directories`, `target_compile_options`, `target_link_options` reference updated accordingly.

### python/CMakeLists.txt
- `reseq_lib` → `reseq2_lib` in `target_link_libraries`

### .github/workflows/ci.yml
- `build/bin/reseq_test` → `build/bin/reseq2_test` in test-isolation job (line 182)

### Verification
```bash
cmake -S . -B build && cmake --build build -j$(nproc)
cd build && ctest --output-on-failure
ls build/bin/reseq2  # binary exists
```

**Commit:** `build: rename CMake targets and binary to reseq2`

---

## Task C: CLI Branding in main.cpp

**File:** `reseq/main.cpp`

### Version output (--version flag)
```
"ReSeq version X.Y" → "ReSeq2 version X.Y.Z"
```
Add `RESEQ_VERSION_PATCH` to the version string.

### Usage block
```
"Program: reseq (REal SEQuence replicator)"
→ "Program: reseq2 (REal SEQuence replicator 2)"

"Usage:  reseq <command> [options]"
→ "Usage:  reseq2 <command> [options]"
```

### Contact line
```
"Contact: Stephan Schmeing <stephan.schmeing@uzh.ch>"
→ "Contact: Bernt Popp (original: Stephan Schmeing <stephan.schmeing@uzh.ch>)"
```

### Running message
```
"Running ReSeq version X.Y" → "Running ReSeq2 version X.Y.Z"
```

### Verification
```bash
build/bin/reseq2 --version     # should print "ReSeq2 version 2.0.0"
build/bin/reseq2                # should show updated usage with reseq2
```

**Commit:** `feat: update CLI branding to ReSeq2`

---

## Task D: Doc CLI References

**Files:** `README.md`, `CLAUDE.md`, `docs/getting-started/quickstart.md`, `docs/user-guide/parameters.md`, `docs/user-guide/commands.md`, `docs/user-guide/error-model.md`, `docs/user-guide/file-formats.md`, `docs/user-guide/faq.md`, `docs/user-guide/profiles.md`

### What changes
All CLI command invocations: `reseq <subcommand>` → `reseq2 <subcommand>`. This includes:
- `reseq illuminaPE` → `reseq2 illuminaPE`
- `reseq seqToIllumina` → `reseq2 seqToIllumina`
- `reseq queryProfile` → `reseq2 queryProfile`
- `reseq replaceN` → `reseq2 replaceN`
- `reseq convertProfile` → `reseq2 convertProfile`

### What does NOT change
- References to `.reseq` file extension (profile files) — these stay as-is
- References to `schmeing/ReSeq` in attribution — these stay as-is
- The `reseq/` source directory in CLAUDE.md architecture section — stays as-is
- The `reseq::` namespace references — stay as-is

### CLAUDE.md specifics
- Update target names: `reseq_lib` → `reseq2_lib`, `reseq_test` → `reseq2_test`
- Update binary references: `build/bin/reseq_test` → `build/bin/reseq2_test`
- Update `reseq` executable description to `reseq2`

### Verification
```bash
# Should only find: .reseq file extension, schmeing/ReSeq attribution,
# reseq/ directory, reseq:: namespace, python/reseq/
grep -rn '\breseq\b' README.md docs/ CLAUDE.md | grep -v '\.reseq' | grep -v 'schmeing' | grep -v 'reseq/' | grep -v 'reseq::' | grep -v 'python/reseq'
```

**Commit:** `docs: update CLI references from reseq to reseq2`

---

## Task E: Git Tag and GitHub Release

**Prerequisite:** Tasks A-D committed and build/tests passing.

### Tag
```bash
git tag -a v2.0.0 -m "ReSeq2 v2.0.0 - first independent release"
```

### Release
Create via `gh release create v2.0.0 --repo berntpopp/ReSeq2` with release notes covering:
- What ReSeq2 is (successor to ReSeq)
- What changed from upstream (C++20, CMake, CI/CD, tests, convertProfile, code quality)
- Installation instructions
- Attribution to Schmeing & Robinson

**Commit:** No commit (tag + release only)

---

## Task F: Tracking Issues

Create two GitHub issues on `berntpopp/ReSeq2`:

1. **JOSS paper** — title: `Write and submit JOSS paper for ReSeq2`, label: `documentation`. Body: brief outline of scope (750-1750 words describing improvements over upstream).

2. **Bioconda recipe** — title: `Submit Bioconda recipe for reseq2`, label: `packaging`. Body: reference the draft structure from the original planning doc, note that sha256 from v2.0.0 tarball is needed.

**Commit:** No commit (GitHub issues only)

---

## Task Order

```
A (version) → B (CMake rename) → C (main.cpp) ─┐
                                                  ├→ E (tag + release) → F (issues)
                                  D (doc refs) ──┘
```

C and D are independent after B and can be done in parallel.
