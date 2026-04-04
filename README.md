# ReSeq2

[![CI](https://github.com/berntpopp/ReSeq2/actions/workflows/ci.yml/badge.svg)](https://github.com/berntpopp/ReSeq2/actions/workflows/ci.yml)
[![codecov](https://codecov.io/gh/berntpopp/ReSeq2/graph/badge.svg)](https://codecov.io/gh/berntpopp/ReSeq2)
[![Docs](https://img.shields.io/badge/docs-berntpopp.github.io%2FReSeq2-blue)](https://berntpopp.github.io/ReSeq2/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

Realistic simulator for genomic DNA sequences from Illumina paired-end sequencers. ReSeq2 learns error, quality, and coverage profiles from real data and uses them to generate synthetic reads with matching k-mer spectra.

> **ReSeq2** is a maintained continuation of [ReSeq](https://github.com/schmeing/ReSeq) by Schmeing & Robinson ([Genome Biology, 2021](https://doi.org/10.1186/s13059-021-02265-7)).

## Quick Start

```bash
git clone https://github.com/berntpopp/ReSeq2.git && cd ReSeq2
cmake -S . -B build && cmake --build build -j$(nproc)

# Full pipeline: learn from real data and simulate
reseq2 illuminaPE -j 32 -r reference.fa -b mappings.bam \
  -1 simulated_R1.fq -2 simulated_R2.fq
```

## Documentation

Full documentation at **[berntpopp.github.io/ReSeq2](https://berntpopp.github.io/ReSeq2/)** — installation, user guide, parameters, file formats, developer docs.

## Citation

Schmeing, S., Robinson, M.D. ReSeq simulates realistic Illumina high-throughput sequencing data. *Genome Biol* **22**, 67 (2021). [doi:10.1186/s13059-021-02265-7](https://doi.org/10.1186/s13059-021-02265-7)

## License

[MIT](LICENSE)
