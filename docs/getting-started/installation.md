# Installation

## Build Requirements

| Requirement | Minimum Version | Ubuntu / Debian |
|---|---|---|
| C++20 compiler (GCC or Clang) | GCC 10+ / Clang 12+ | `sudo apt install build-essential` |
| CMake | 3.16+ | `sudo apt install cmake` |
| Boost (serialization, program_options, filesystem, system, math, iostreams) | 1.48+ | `sudo apt install libboost-all-dev` |
| ZLIB | --- | `sudo apt install zlib1g-dev` |
| BZip2 | --- | `sudo apt install libbz2-dev` |
| Git | --- | `sudo apt install git` |

!!! note "Automatically fetched dependencies"
    SeqAn 2.5.2, GoogleTest, and NLopt are fetched automatically at configure time via CMake `FetchContent`. If they are already installed on your system, CMake will find them with `find_package()` instead.

## Building from Source

```bash
git clone https://github.com/berntpopp/ReSeq2.git
cd ReSeq2
cmake -S . -B build
cmake --build build -j$(nproc)
```

Run the test suite to verify the build:

```bash
ctest --test-dir build --output-on-failure
```

The executable will be at `build/bin/reseq2`.

## System-wide Installation

```bash
# Install to /usr/local (default)
cmake --install build

# Or install to a custom prefix
cmake --install build --prefix /your/custom/prefix
```

## Optional: Python Plotting Tools

ReSeq2 ships optional Python utilities for plotting and read-name preparation. To build the Python bindings, you need additional dependencies:

| Requirement | Minimum Version | Ubuntu / Debian |
|---|---|---|
| Python | 3.9+ | (usually pre-installed) |
| python3-dev | --- | `sudo apt install python3-dev` |
| SWIG | 3+ | `sudo apt install swig` |

Enable Python bindings at configure time:

```bash
cmake -S . -B build -DRESEQ_BUILD_PYTHON=ON
cmake --build build -j$(nproc)
```

Install the Python package:

```bash
pip install .
```

This provides the `reseq-prepare-names` tool used for [tile-aware mapping](quickstart.md#tile-aware-mapping).

## Bioconda

!!! warning "Bioconda package status"
    The current Bioconda `reseq` package installs the **unmaintained upstream** version. A dedicated `reseq2` Bioconda package is planned for a future release. In the meantime, build from source for the latest features and fixes.

If you still want to install the legacy upstream version via Conda:

```bash
conda install -c bioconda -c conda-forge reseq
```
