# FAQ

Frequently asked questions about ReSeq2.

??? question "Can I simulate more than the default 128 alleles?"
    Yes. Set `kMaxAlleles` in `reseq/Reference.h` to any multiple of 64. After recompilation the new maximum number of alleles will be your chosen value.

    ```cpp
    // reseq/Reference.h
    static constexpr uintAlleleBitArray kMaxAlleles = 256;  // was 128
    ```

    Then rebuild:

    ```bash
    cmake --build build -j$(nproc)
    ```

??? question "Can I simulate exome sequencing?"
    Yes. Create a reference that contains only the exons as individual scaffolds. Use `--refBiasFile` to specify the coverage of individual exons.

    To simulate intron contamination, append the whole genome reference to the exon-only reference and assign a very low coverage bias to the full-genome scaffolds via `--refBiasFile`.

??? question "Can I train on datasets without adapters?"
    Generally, it is not advised to use trimmed datasets, because they result in worse performance. However, by specifying decoy adapters with `--adapterFile TruSeq_single` you can skip the automatic adapter detection, which otherwise will prevent you from training on datasets without adapters.

??? question "When I train the model, a large part of the genome is excluded because the sequences are too short."
    Lowering the `--maxFragLen` parameter most likely helps in this situation, because sequences that are not at least 100 bases longer than this parameter are excluded in any case.

    !!! warning
        Check that you are not truncating your fragment length distribution by setting `--maxFragLen` too low. Inspect the fragment length bias output with `reseq queryProfile --fragLenBias` to verify.
