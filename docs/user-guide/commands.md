# Commands

ReSeq2 provides five commands, each accessible as a sub-command of the `reseq` binary.

```
reseq <command> [options]
```

## illuminaPE

Full paired-end simulation pipeline. This is the primary command and covers the entire workflow: collecting sequencing statistics from a BAM file, estimating multi-dimensional probability distributions via Iterative Proportional Fitting, and simulating realistic paired-end reads. Each stage can be run independently by combining the appropriate flags (`--statsOnly`, `--stopAfterEstimation`, `--ipfIterations 0`).

```bash
reseq illuminaPE -j 32 -r ref.fa -b reads.bam -1 sim_R1.fq -2 sim_R2.fq
```

## seqToIllumina

Apply the Illumina error and quality model directly to input sequences. Use this when the coverage model is not applicable --- for example, when simulating reads from synthetic or designed sequences. It adds quality scores, substitution errors, and InDel errors, and trims sequences to the read length (appending adapter sequence if the input is shorter).

```bash
reseq seqToIllumina -j 2 -i sequences.fa -o reads.fq -s profile.reseq
```

See [Error Model](error-model.md) for the required input FASTA format and workflow details.

## queryProfile

Extract summary information from a `.reseq` stats file. Useful for inspecting fragment-length bias, reference-sequence bias, maximum read length, or longest detected deletion without running a full simulation.

```bash
reseq queryProfile -s profile.reseq --fragLenBias bias.tsv
```

## replaceN

Replace ambiguous bases (N and other IUPAC codes) in a reference FASTA with random A/C/G/T. Running this once before simulation ensures that the same replacement is used consistently across multiple simulation runs with the same reference.

```bash
reseq replaceN -r ref_with_ns.fa -R ref_clean.fa --seed 42
```

## convertProfile

Convert stats and probability files between binary and text formats. Binary format is the default (smaller, faster) but is not portable across CPU architectures or compilers. Text format is portable and suitable for sharing or archiving.

```bash
# Binary to text
reseq convertProfile -s profile.reseq --textFormat

# Text to binary
reseq convertProfile -s profile.reseq
```

See [Profiles](profiles.md) for a detailed discussion of format trade-offs.

---

For the full list of options for each command, see [Parameters](parameters.md).
