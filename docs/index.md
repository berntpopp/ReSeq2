# ReSeq2

**Realistic Illumina sequencing simulator**

ReSeq2 closes the gap between simulated and real sequencing data by learning comprehensive error and quality profiles directly from real Illumina paired-end runs. Unlike conventional simulators that rely on simplified parametric models, ReSeq2 captures the full complexity of a sequencing experiment --- coverage profiles, systematic errors, tile effects, GC bias, and the k-mer spectrum --- and faithfully reproduces them in synthetic reads.

By combining simulated and real data in benchmarks, researchers gain two complementary perspectives on tool performance, reducing the bias that comes from relying on either alone. ReSeq2 accepts any BAM file mapped to a reference genome, estimates multi-dimensional probability distributions via Iterative Proportional Fitting, and produces paired-end FASTQ files that closely match the statistical fingerprint of the original run.

## Pipeline Overview

```mermaid
flowchart LR
    BAM[BAM + Reference] --> Stats[Stats Collection]
    Stats --> IPF[Probability Estimation]
    IPF --> Sim[Read Simulation]
    Sim --> FQ[FASTQ Output]

    style BAM fill:#00796b,color:#fff
    style Stats fill:#00897b,color:#fff
    style IPF fill:#009688,color:#fff
    style Sim fill:#26a69a,color:#fff
    style FQ fill:#4db6ac,color:#fff
```

## Key Features

- **Learns from real data** --- profiles are extracted directly from your BAM files, not hand-tuned parameters
- **Reproduces the k-mer spectrum** --- synthetic reads match the k-mer distribution of the original run
- **Coverage profiles** --- GC bias, fragment-length bias, and reference-sequence bias are all modelled
- **Systematic errors** --- position- and context-dependent error patterns are preserved
- **Tile support** --- per-tile quality variation can be captured and simulated
- **VCF variation** --- simulate diploid genomes or populations by providing a VCF file
- **Methylation** --- bisulfite sequencing simulation with per-allele methylation levels
- **Profile sharing** --- portable text format for cross-platform reproducibility

## Next Steps

<div class="grid cards" markdown>

-   :material-rocket-launch:{ .lg .middle } **Getting Started**

    ---

    Install ReSeq2 and run your first simulation in minutes.

    [:octicons-arrow-right-24: Installation](getting-started/installation.md){ .md-button }
    [:octicons-arrow-right-24: Quick Start](getting-started/quickstart.md){ .md-button }

-   :material-book-open-variant:{ .lg .middle } **User Guide**

    ---

    Commands, parameters, file formats, and advanced topics.

    [:octicons-arrow-right-24: Commands](user-guide/commands.md){ .md-button }
    [:octicons-arrow-right-24: Parameters](user-guide/parameters.md){ .md-button }

</div>
