# Quick Start

This guide walks you through a complete simulation workflow: mapping real reads, collecting statistics, estimating probabilities, and generating synthetic FASTQ files.

## Step 1: Map Real Data

ReSeq2 needs a position-sorted BAM file of your real reads mapped to a reference genome. For example, using `bowtie2`:

```bash
# Build the reference index
bowtie2-build my_reference.fa my_reference

# Map paired-end reads and sort
bowtie2 -p 32 -X 2000 -x my_reference \
  -1 my_data_1.fq -2 my_data_2.fq \
  | samtools sort -m 10G -@ 4 -T _tmp -o my_mappings.bam -
```

!!! tip
    Any aligner that produces valid BAM output will work. The `-X 2000` flag in `bowtie2` allows fragments up to 2000 bp, matching ReSeq2's default `--maxFragLen`.

## Step 2: Run the Full Pipeline

The simplest way to simulate is a single `illuminaPE` command that runs all three stages --- stats collection, probability estimation, and simulation:

```bash
reseq2 illuminaPE -j 32 \
  -r my_reference.fa \
  -b my_mappings.bam \
  -1 my_simulated_data_1.fq \
  -2 my_simulated_data_2.fq
```

That is it. Two FASTQ files with realistic simulated reads are now ready.

## Step-by-Step Approach

For large projects you may want to run each stage independently, for example to simulate multiple times from the same profile without re-collecting statistics.

=== "1. Collect Stats"

    ```bash
    reseq2 illuminaPE -j 32 \
      -r my_reference.fa \
      -b my_mappings.bam \
      --statsOnly
    ```

    Produces `my_mappings.bam.reseq` (the stats profile).

=== "2. Estimate Probabilities"

    ```bash
    reseq2 illuminaPE -j 32 \
      -s my_mappings.bam.reseq \
      --stopAfterEstimation
    ```

    Produces `my_mappings.bam.reseq.ipf` (the probability estimates).

=== "3. Simulate Reads"

    ```bash
    reseq2 illuminaPE -j 32 \
      -R my_reference.fa \
      -s my_mappings.bam.reseq \
      --ipfIterations 0 \
      -1 my_simulated_data_1.fq \
      -2 my_simulated_data_2.fq
    ```

    Uses `--ipfIterations 0` to skip re-estimation and load the cached `.ipf` file.

## Adding Variation with VCF

To simulate diploid genomes or populations, provide a VCF file with the `-V` flag:

```bash
reseq2 illuminaPE -j 32 \
  -r my_reference.fa \
  -b my_mappings.bam \
  -V my_variation.vcf \
  -1 my_simulated_data_1.fq \
  -2 my_simulated_data_2.fq
```

Or, when simulating from a pre-built profile:

```bash
reseq2 illuminaPE -j 32 \
  -R my_reference.fa \
  -s my_mappings.bam.reseq \
  -V my_variation.vcf \
  --ipfIterations 0 \
  -1 my_simulated_data_1.fq \
  -2 my_simulated_data_2.fq
```

## Tile-Aware Mapping

Illumina tile information improves simulation realism but is often lost during mapping because read names contain a space before the tile field. The `reseq-prepare-names` tool (installed via `pip install .`) replaces the space with an underscore on the fly:

```bash
bowtie2 -p 32 -X 2000 -x my_reference \
  -1 <(reseq-prepare-names my_data_1.fq my_data_2.fq) \
  -2 <(reseq-prepare-names my_data_2.fq my_data_1.fq) \
  | samtools sort -m 10G -@ 4 -T _tmp -o my_mappings.bam -
```

Then enable tiles during stats collection:

```bash
reseq2 illuminaPE -j 32 \
  -r my_reference.fa \
  -b my_mappings.bam \
  --tiles \
  --statsOnly
```

## Using Pre-built Profiles

For best results, create your own profiles from a dataset that closely matches your target sequencer, chemistry, fragmentation, adapters, and PCR cycles. Training on the same or a closely related species ensures that the profile space (e.g., GC content range) is well populated.

When a wide variety of conditions matters more than exact matching, the [ReSeq-profiles repository](https://github.com/schmeing/ReSeq-profiles) provides curated, high-quality profiles with detailed metadata on the original datasets.

!!! tip "Verify simulation quality"
    Whenever possible, benchmark your downstream tools on both the simulated and the original dataset to confirm that the simulation created realistic conditions for your use case.
