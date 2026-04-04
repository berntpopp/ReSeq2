# Parameters

Complete option reference for every ReSeq2 command. Select a tab to view the parameters for a specific command.

=== "illuminaPE"

    ```
    reseq illuminaPE [options]
    ```

    **General**

    | Parameter | Default | Description |
    |---|---|---|
    | `-h` `--help` | | Prints help information and exits |
    | `-j` `--threads` | 0 | Number of threads used (0=auto) |
    | `--verbosity` | 4 | Sets the level of verbosity (4=everything, 0=nothing) |
    | `--version` | | Prints version info and exits |

    **Stats**

    | Parameter | Default | Description |
    |---|---|---|
    | `--adapterFile` | (AutoDetect) | Fasta file with adapter sequences |
    | `--adapterMatrix` | (AutoDetect) | 0/1 matrix with valid adapter pairing (first read in rows, second read in columns) |
    | `-b` `--bamIn` | None | Position sorted bam/sam file with reads mapped to `--refIn` |
    | `--binSizeBiasFit` | 100000000 | Reference sequences larger than this are split for bias fitting to limit memory consumption |
    | `--maxFragLen` | 2000 | Maximum fragment length to include pairs into statistics |
    | `--minMapQ` | 10 | Minimum mapping quality to include pairs into statistics |
    | `--noBias` | | Do not perform bias fit. Results in uniform coverage if simulated from |
    | `--noTiles` | | Ignore tiles for the statistics [default] |
    | `-r` `--refIn` | None | Reference sequences in fasta format (gz and bz2 supported) |
    | `--statsOnly` | | Only generate the statistics |
    | `-s` `--statsIn` | None | Skips statistics generation and reads directly from stats file |
    | `-S` `--statsOut` | `--bamIn`.reseq | Stores the real data statistics for reuse in given file |
    | `--tiles` | | Use tiles for the statistics |
    | `-v` `--vcfIn` | None | Ignore all positions with a listed variant for stats generation |
    | `--textFormat` | | Write profile files in portable text format instead of compressed binary |
    | `--bothFormats` | | Write profiles in both binary and text formats (alternate gets `.text` or `.bin` suffix) |

    **Probabilities**

    | Parameter | Default | Description |
    |---|---|---|
    | `--ipfIterations` | 200 | Maximum number of iterations for iterative proportional fitting |
    | `--ipfPrecision` | 5 | Iterative proportional fitting procedure stops after reaching this precision (%) |
    | `-p` `--probabilitiesIn` | `--statsIn`.ipf | Loads last estimated probabilities and continues from there if precision is not met |
    | `-P` `--probabilitiesOut` | `--probabilitiesIn` | Stores the probabilities estimated by iterative proportional fitting |
    | `--stopAfterEstimation` | | Stop after estimating the probabilities |

    **Simulation**

    | Parameter | Default | Description |
    |---|---|---|
    | `-1` `--firstReadsOut` | reseq-R1.fq | Writes the simulated first reads into this file |
    | `-2` `--secondReadsOut` | reseq-R2.fq | Writes the simulated second reads into this file |
    | `-c` `--coverage` | 0 | Approximate average read depth simulated (0 = Corrected original coverage) |
    | `--errorMutliplier` | 1.0 | Divides the original probability of correct base calls (no substitution error) by this value and renormalizes |
    | `--methylation` | | Extended bed graph file specifying methylation for regions. Multiple score columns for individual alleles are possible, but must match with vcfSim. C->T conversions for 1-specified value in region. |
    | `--noInDelErrors` | | Simulate reads without InDel errors |
    | `--noSubstitutionErrors` | | Simulate reads without substitution errors |
    | `--numReads` | 0 | Approximate number of read pairs simulated (0 = Use `--coverage`) |
    | `--readSysError` | None | Read systematic errors from file in fastq format (seq=dominant error, qual=error percentage) |
    | `--recordBaseIdentifier` | ReseqRead | Base identifier for the simulated fastq records, followed by a count and other information about the read |
    | `--refBias` | keep/no | Way to select the reference biases for simulation (keep [from refIn] / no [biases] / draw [with replacement from original biases] / file) |
    | `--refBiasFile` | None | File to read reference biases from: one sequence per line (identifier bias) |
    | `-R` `--refSim` | `--refIn` | Reference sequences in fasta format to simulate from |
    | `--seed` | None | Seed used for simulation; if none is given a random seed will be used |
    | `-V` `--vcfSim` | None | Defines genotypes to simulate alleles or populations |
    | `--writeSysError` | None | Write the randomly drawn systematic errors to file in fastq format (seq=dominant error, qual=error percentage) |

=== "queryProfile"

    ```
    reseq queryProfile [options]
    ```

    **General**

    | Parameter | Default | Description |
    |---|---|---|
    | `-h` `--help` | | Prints help information and exits |
    | `-j` `--threads` | 0 | Number of threads used (0=auto) |
    | `--verbosity` | 4 | Sets the level of verbosity (4=everything, 0=nothing) |
    | `--version` | | Prints version info and exits |

    **queryProfile**

    | Parameter | Default | Description |
    |---|---|---|
    | `--fragLenBias` | None | Output fragment length bias to file (tsv format; `-` for stdout) |
    | `--maxLenDeletion` | None | Output lengths of longest detected deletion to stdout |
    | `--maxReadLength` | None | Output lengths of longest detected read to stdout |
    | `-r` `--ref` | None | Reference sequences in fasta format (gz and bz2 supported) |
    | `--refSeqBias` | None | Output reference sequence bias to file (tsv format; `-` for stdout) |
    | `-s` `--stats` | None | Reseq statistics file to extract reference sequence bias |

=== "replaceN"

    ```
    reseq replaceN [options]
    ```

    **General**

    | Parameter | Default | Description |
    |---|---|---|
    | `-h` `--help` | | Prints help information and exits |
    | `-j` `--threads` | 0 | Number of threads used (0=auto) |
    | `--verbosity` | 4 | Sets the level of verbosity (4=everything, 0=nothing) |
    | `--version` | | Prints version info and exits |

    **replaceN**

    | Parameter | Default | Description |
    |---|---|---|
    | `-r` `--refIn` | None | Reference sequences in fasta format (gz and bz2 supported) |
    | `-R` `--refSim` | None | File to where reference sequences in fasta format with Ns randomly replaced should be written |
    | `--seed` | None | Seed used for replacing N; if none is given a random seed will be used |

=== "seqToIllumina"

    ```
    reseq seqToIllumina [options]
    ```

    **General**

    | Parameter | Default | Description |
    |---|---|---|
    | `-h` `--help` | | Prints help information and exits |
    | `-j` `--threads` | 0 | Number of threads used (0=auto) |
    | `--verbosity` | 4 | Sets the level of verbosity (4=everything, 0=nothing) |
    | `--version` | | Prints version info and exits |

    **seqToIllumina**

    | Parameter | Default | Description |
    |---|---|---|
    | `--errorMutliplier` | 1.0 | Divides the original probability of correct base calls (no substitution error) by this value and renormalizes |
    | `-i` `--input` | `stdin` | Input file (fasta format, gz and bz2 supported) |
    | `--ipfIterations` | 200 | Maximum number of iterations for iterative proportional fitting |
    | `--ipfPrecision` | 5 | Iterative proportional fitting procedure stops after reaching this precision (%) |
    | `--noInDelErrors` | | Simulate reads without InDel errors |
    | `--noSubstitutionErrors` | | Simulate reads without substitution errors |
    | `-o` `--output` | `stdout` | Output file (fastq format, gz and bz2 supported) |
    | `-p` `--probabilitiesIn` | `--statsIn`.ipf | Loads last estimated probabilities and continues from there if precision is not met |
    | `-P` `--probabilitiesOut` | `--probabilitiesIn` | Stores the probabilities estimated by iterative proportional fitting |
    | `--seed` | None | Seed used for simulation; if none is given a random seed will be used |
    | `-s` `--statsIn` | None | Profile file that contains the statistics used for simulation |
    | `--textFormat` | | Write profile files in portable text format instead of compressed binary |
    | `--bothFormats` | | Write profiles in both binary and text formats (alternate gets `.text` or `.bin` suffix) |

=== "convertProfile"

    ```
    reseq convertProfile [options]
    ```

    **General**

    | Parameter | Default | Description |
    |---|---|---|
    | `-h` `--help` | | Prints help information and exits |
    | `-j` `--threads` | 0 | Number of threads used (0=auto) |
    | `--verbosity` | 4 | Sets the level of verbosity (4=everything, 0=nothing) |
    | `--version` | | Prints version info and exits |

    **convertProfile**

    | Parameter | Default | Description |
    |---|---|---|
    | `-s` `--statsIn` | None | Input stats file (.reseq) |
    | `-o` `--statsOut` | Overwrites input | Output stats file |
    | `-p` `--probsIn` | None | Input probabilities file (.reseq.ipf) |
    | `-P` `--probsOut` | Overwrites input | Output probabilities file |
    | `--textFormat` | | Write in portable text format (default: compressed binary) |
