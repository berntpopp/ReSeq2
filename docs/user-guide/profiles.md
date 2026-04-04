# Profiles

ReSeq2 stores sequencing statistics and estimated probabilities as **profile files**. Understanding the two available formats helps you choose the right one for your workflow.

## Binary vs Text Format

| | Binary (default) | Text (`--textFormat`) |
|---|---|---|
| **File size** | ~65% smaller | Larger |
| **Load speed** | ~2--3x faster | Slower |
| **Portable** | No | Yes |
| **Use case** | Local workflows on a single machine | Sharing, archiving, cross-platform use |

!!! warning "Binary portability"
    Binary profiles are Boost serialization archives. They are **not portable** across different CPU architectures or compilers. A profile generated on x86 with GCC may not load on ARM or with Clang. Always use text format when sharing profiles with collaborators or publishing them.

Both formats are auto-detected on load --- you never need to specify which format a file is in when reading.

## Converting Between Formats

Use the `convertProfile` command to convert existing profile files.

=== "Binary to text"

    ```bash
    reseq convertProfile -s my_mappings.bam.reseq --textFormat
    ```

    This overwrites the input file with the text-format version.

=== "Text to binary"

    ```bash
    reseq convertProfile -s my_mappings.bam.reseq
    ```

    Without `--textFormat`, the default binary format is used.

=== "Specify output path"

    ```bash
    reseq convertProfile -s input.reseq -o output.reseq --textFormat
    ```

    Write to a different file instead of overwriting.

Probability files (`.reseq.ipf`) can be converted the same way using `-p` and `-P`:

```bash
reseq convertProfile -p my_mappings.bam.reseq.ipf -P output.reseq.ipf --textFormat
```

## Generating Both Formats at Once

During stats collection, use `--bothFormats` to produce both binary and text profiles simultaneously. The alternate format gets a `.text` or `.bin` suffix:

```bash
reseq illuminaPE -j 32 -r my_reference.fa -b my_mappings.bam \
  --statsOnly --bothFormats
```

This creates:

- `my_mappings.bam.reseq` (binary, default)
- `my_mappings.bam.reseq.text` (text, portable)

## Using Pre-built Profiles

The [ReSeq-profiles repository](https://github.com/schmeing/ReSeq-profiles) provides curated profiles with detailed metadata about the original datasets. These are useful when:

- You need a wide variety of sequencing conditions for benchmarking
- You do not have access to a closely matching real dataset
- You want to quickly test a pipeline without running the full stats collection

!!! tip "Best practice"
    For the most realistic simulations, create your own profiles from a dataset that closely matches your target sequencer, chemistry, fragmentation protocol, adapters, and PCR cycles. Pre-built profiles are a good fallback but may not capture the specific characteristics of your experiment.

## Cross-Platform Sharing Advice

When distributing profiles (e.g., alongside a publication or benchmark):

1. **Always share text format** --- it loads on any platform regardless of compiler or architecture
2. **Include the `.reseq.ipf` file** alongside the `.reseq` stats file so recipients can skip probability estimation
3. **Document the original dataset** --- species, sequencer model, chemistry version, and read length help users assess whether a profile is appropriate for their use case
