# Error Model (seqToIllumina)

The `seqToIllumina` command applies the Illumina error and quality model directly to input sequences, bypassing the coverage model. This is useful when you have specific sequences you want converted to realistic reads --- for example, synthetic constructs, designed amplicons, or sequences from a custom simulation pipeline.

## When to Use seqToIllumina

Use `seqToIllumina` instead of `illuminaPE` when:

- You already have the exact sequences you want as reads (no coverage model needed)
- You want to add realistic quality scores, substitution errors, and InDel errors to known sequences
- You are simulating reads from sequences shorter than the read length (adapters are appended automatically)

## Basic Command

```bash
reseq seqToIllumina -j 2 \
  -i my_sequences.fa \
  -o my_simulated_reads.fq \
  -s my_stats_profile.reseq
```

## Input FASTA Format

Each input sequence must provide all the information needed by the error and quality model. The required format is:

```
>{sequence id} {template segment};{fragment length};{error tendencies};{error rates}
{sequence to convert}
```

### Field Descriptions

**`{sequence id}`**
:   The desired read identifier. May contain spaces. The output FASTQ record will have the description:
    `@{sequence id} {cigar} E{number of errors in read}`

**`{template segment}`**
:   `1` for first reads (R1) or `2` for second reads (R2).

**`{sequence to convert}`**
:   The nucleotide sequence to which errors and qualities will be added. It may only contain `A`, `C`, `G`, or `T`.

    !!! warning
        Ns are not permitted in the input sequence. Ambiguous bases must be resolved consistently for all reads from a given reference position. Use [`reseq replaceN`](commands.md#replacen) to pre-process your reference.

**`{error tendencies}`**
:   Must be the same length as the sequence. Encodes the dominant systematic error at each position. All bases stemming from the same position and strand in the reference must have identical values. For insertions specific to one read, use `N`.

**`{error rates}`**
:   Must be the same length as the sequence. Encodes the systematic error rate in percent at each position (quality-encoded with offset 33). All bases from the same reference position and strand must have identical values. For read-specific insertions, use `!`.

!!! note
    The format of error tendencies and error rates matches the [Systematic Error File](file-formats.md#systematic-error-file-fq) format. See that section for encoding details.

## Generating Systematic Errors for a Reference

To obtain the error tendencies and rates needed for your input sequences, generate a systematic error file from a stats profile:

```bash
reseq illuminaPE \
  -r my_reference.fa \
  -s my_stats_profile.reseq \
  --stopAfterEstimation \
  --writeSysError my_systematic_errors.fq
```

This produces a FASTQ file with two entries per reference sequence:

1. **Reverse strand** (listed first) --- reverse complemented
2. **Forward strand** (listed second) --- taken as-is

Extract the corresponding error tendencies and rates from this file when constructing your `seqToIllumina` input.

!!! tip "Reverse strand handling"
    The reverse-strand entry is already reverse complemented. An `A` in the first position of the reverse-strand entry means a systematic error towards `T` is simulated at the **last** base of the reference sequence for reverse-strand reads.
