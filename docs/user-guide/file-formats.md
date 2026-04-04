# File Formats

ReSeq2 reads and writes several file formats. This page describes each format, its file extension, and the conventions ReSeq2 expects.

## Reference File (`.fa`)

Standard FASTA format reference genome. Compressed files (`.fa.gz`, `.fa.bz2`) are also accepted.

Any character that is not A, C, G, or T is randomly replaced by one of those four bases before simulation. To ensure consistent replacement across multiple simulation runs, pre-process your reference once with the [`replaceN`](commands.md#replacen) command.

!!! note "Maximum sequence length"
    A single reference sequence is supported up to a maximum length of 4,294,967,295 bases (2^32 - 1). The complete reference genome across all sequences can be much larger.

## Mapping File (`.bam`)

Standard BAM format alignment file. The reference information in the BAM header must match the provided reference FASTA.

- Only **primary alignments** are used; supplementary and secondary alignments are ignored.
- To include tile information, the tile field must remain in the QNAME (read name). See [Tile-Aware Mapping](../getting-started/quickstart.md#tile-aware-mapping) for how to preserve it during alignment.

## Adapter File (`.fa`)

FASTA file listing the sequences of possibly used adapters. Keep this list short to reduce false identifications. Example adapter files are provided in the `adapters/` directory of the repository.

!!! tip
    The strand direction of adapters does not matter --- ReSeq2 always checks both the given sequence and its reverse complement, since directionality depends on the sequencing machine.

## Adapter Matrix (`.mat`)

A 0/1 matrix specifying which adapter pairs can co-occur in a read pair.

- `1` = valid pairing, `0` = not valid
- Rows represent adapters in the first read; columns represent adapters in the second read
- Columns are consecutive digits within a row (no delimiter)
- The number of rows and columns must match the number of entries in the adapter file

**Example** for three adapters:

```
110
101
011
```

## Variant File (`.vcf`)

Standard VCF format. Requirements and conventions:

- The reference information in the VCF header and the `REF` column must match the provided reference FASTA
- Ambiguous bases (e.g., N) are **not supported** in `REF` or `ALT` columns
- Only the `CHROM`, `POS`, `ALT` columns and genotype fields are used
- No quality filtering is applied --- all genotypes in the file are simulated
- Genotypes across all samples are combined; no distinction is made between single-sample and multi-sample VCFs
- All genotype information is treated as phased, regardless of how it is encoded in the file
- A maximum of 128 genotypes is supported by default (see [FAQ](faq.md))

## Methylation File (`.bed`)

Extended BedGraph format specifying methylation levels for genomic regions.

- Multiple score columns for individual alleles are supported; the number of alleles must match the VCF file
- The number of score columns must be consistent within each reference sequence (either 1 for all alleles, or one per allele)
- Bisulfite sequencing is simulated: C-to-T conversions are inserted with probability `1 - methylation_value`

## Stats File (`.reseq`)

Boost archive containing sequencing statistics collected from the input BAM. This is the main profile file.

| Format | Flag | Size | Speed | Portable |
|---|---|---|---|---|
| Binary (default) | _(none)_ | ~65% smaller | ~2--3x faster to load | No --- architecture/compiler dependent |
| Text | `--textFormat` | Larger | Slower | Yes |

Both formats are auto-detected on load. Use [`convertProfile`](commands.md#convertprofile) or the `--bothFormats` flag to switch between them. See [Profiles](profiles.md) for details.

## Probability File (`.reseq.ipf`)

Boost archive containing the probability distributions estimated by Iterative Proportional Fitting. Same format options and portability considerations as the stats file.

## Systematic Error File (`.fq`)

Standard FASTQ format encoding position-specific systematic errors for a reference genome.

- **Sequence field**: the dominant error tendency at each position
- **Quality field**: the error rate in percent at that position (encoded as quality + 33 offset)
- Two entries per reference sequence (one per strand):
    1. **First entry** = reverse strand, reverse complemented. An `A` in the first position means a systematic error towards `T` at the last base of the reference sequence for reverse-strand reads.
    2. **Second entry** = forward strand, taken as-is (not reverse complemented).
- Entry lengths must match the corresponding reference sequence lengths
- The order of reference sequences must be preserved

!!! note "Quality encoding for high error rates"
    Since FASTQ quality values are limited to 94 levels, odd percentages above 86% are omitted. `~` (ASCII 126) encodes 100% and `}` (ASCII 125) encodes 98%.

Generate systematic errors for a reference with:

```bash
reseq2 illuminaPE -r my_reference.fa -s my_profile.reseq \
  --stopAfterEstimation --writeSysError my_systematic_errors.fq
```

## Reference Bias File (`.txt`)

Plain text file defining per-sequence coverage biases for simulation.

- One line per reference sequence
- Each line starts with the unique sequence identifier (the part before the first space in the FASTA header)
- The identifier may be followed by a space and arbitrary description text
- The line ends with a space or tab followed by a positive floating-point bias value
- All reference sequences in the reference FASTA must have an entry
- Extra entries for sequences not in the reference are allowed
- Order does not need to match the reference file
- Biases are automatically normalized and define relative base coverage

!!! warning
    Simulated base coverage will differ from the specified biases because other bias models (GC, fragment length, etc.) are applied on top.

**Example:**

```text
chr1 Chromosome 1 1.2
chr2 Chromosome 2 0.8
chrX X chromosome 0.5
```
