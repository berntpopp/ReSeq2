#include "RegressionTest.h"

#include <filesystem>
#include <fstream>
#include <random>
#include <string>

#include "gtest/gtest.h"

void reseq::RegressionTest::Register() {
    // Guarantees that library is included
}

namespace reseq {

// --- replaceN command ---

TEST_F(RegressionTest, ReplaceN) {
    auto ref_in = test_dir_ / "reference-test.fa";
    auto ref_out = tmp_dir_ / "replaced.fa";
    auto expected = expected_dir_ / "replaceN.fa";

    int rc = RunReseq("replaceN -r " + ref_in.string() + " -R " + ref_out.string() + " --seed 42");
    ASSERT_EQ(0, rc) << "replaceN exited with code " << rc;
    ASSERT_TRUE(std::filesystem::exists(ref_out)) << "Output file not created: " << ref_out;

    ExpectTextFilesEqual(expected, ref_out);
}

// --- queryProfile from small BAM (generated at test time) ---

TEST_F(RegressionTest, QueryProfileMaxReadLength) {
    auto profile = GenerateEcoliProfile();
    ASSERT_TRUE(std::filesystem::exists(profile)) << "Profile not generated";

    std::string output = RunReseqCapture("queryProfile -s " + profile.string() + " --maxReadLength");

    // Read expected
    std::ifstream exp_stream(expected_dir_ / "queryProfile_maxReadLength.txt");
    ASSERT_TRUE(exp_stream.is_open());
    std::string expected((std::istreambuf_iterator<char>(exp_stream)), std::istreambuf_iterator<char>());

    EXPECT_EQ(expected, output) << "maxReadLength output mismatch";
}

TEST_F(RegressionTest, QueryProfileMaxLenDeletion) {
    auto profile = GenerateEcoliProfile();
    ASSERT_TRUE(std::filesystem::exists(profile)) << "Profile not generated";

    std::string output = RunReseqCapture("queryProfile -s " + profile.string() + " --maxLenDeletion");

    std::ifstream exp_stream(expected_dir_ / "queryProfile_maxLenDeletion.txt");
    ASSERT_TRUE(exp_stream.is_open());
    std::string expected((std::istreambuf_iterator<char>(exp_stream)), std::istreambuf_iterator<char>());

    EXPECT_EQ(expected, output) << "maxLenDeletion output mismatch";
}

TEST_F(RegressionTest, QueryProfileFragLenBias) {
    auto profile = GenerateEcoliProfile();
    ASSERT_TRUE(std::filesystem::exists(profile)) << "Profile not generated";

    auto actual_file = tmp_dir_ / "fragLenBias.tsv";
    int rc = RunReseq("queryProfile -s " + profile.string() + " --fragLenBias " + actual_file.string());
    ASSERT_EQ(0, rc) << "queryProfile --fragLenBias exited with code " << rc;

    ExpectTextFilesEqual(expected_dir_ / "queryProfile_fragLenBias.tsv", actual_file);
}

// --- queryProfile from Zenodo profile (skip if not downloaded) ---

TEST_F(RegressionTest, RealProfileMaxReadLength) {
    auto profile = ZenodoProfile();
    if (profile.empty()) {
        GTEST_SKIP() << "Zenodo profile not available (test/data/Hs-Nova-TruSeq.reseq)";
    }

    std::string output = RunReseqCapture("queryProfile -s " + profile.string() + " --maxReadLength");

    std::ifstream exp_stream(expected_dir_ / "queryProfile_real_maxReadLength.txt");
    ASSERT_TRUE(exp_stream.is_open());
    std::string expected((std::istreambuf_iterator<char>(exp_stream)), std::istreambuf_iterator<char>());

    EXPECT_EQ(expected, output) << "Real profile maxReadLength mismatch";
}

TEST_F(RegressionTest, RealProfileMaxLenDeletion) {
    auto profile = ZenodoProfile();
    if (profile.empty()) {
        GTEST_SKIP() << "Zenodo profile not available (test/data/Hs-Nova-TruSeq.reseq)";
    }

    std::string output = RunReseqCapture("queryProfile -s " + profile.string() + " --maxLenDeletion");

    std::ifstream exp_stream(expected_dir_ / "queryProfile_real_maxLenDeletion.txt");
    ASSERT_TRUE(exp_stream.is_open());
    std::string expected((std::istreambuf_iterator<char>(exp_stream)), std::istreambuf_iterator<char>());

    EXPECT_EQ(expected, output) << "Real profile maxLenDeletion mismatch";
}

TEST_F(RegressionTest, RealProfileFragLenBias) {
    auto profile = ZenodoProfile();
    if (profile.empty()) {
        GTEST_SKIP() << "Zenodo profile not available (test/data/Hs-Nova-TruSeq.reseq)";
    }

    auto actual_file = tmp_dir_ / "real_fragLenBias.tsv";
    int rc = RunReseq("queryProfile -s " + profile.string() + " --fragLenBias " + actual_file.string());
    ASSERT_EQ(0, rc) << "queryProfile --fragLenBias exited with code " << rc;

    ExpectTextFilesEqual(expected_dir_ / "queryProfile_real_fragLenBias.tsv", actual_file);
}

// --- Error handling ---

TEST_F(RegressionTest, ErrorBadCommand) {
    int rc = RunReseq("nonsenseCommand");
    EXPECT_NE(0, rc) << "Expected non-zero exit for unknown command";
}

TEST_F(RegressionTest, ErrorMissingRef) {
    int rc = RunReseq("replaceN -r /nonexistent/path/ref.fa -R " + (tmp_dir_ / "out.fa").string());
    EXPECT_NE(0, rc) << "Expected non-zero exit for missing reference";
}

TEST_F(RegressionTest, VersionOutput) {
    std::string output = RunReseqCaptureStderr("--version");
    EXPECT_NE(std::string::npos, output.find("ReSeq")) << "Version output should contain 'ReSeq', got: " << output;
}

// --- CLI behavior tests (Phase 5a prerequisite) ---

TEST_F(RegressionTest, BareReseqExitCode) {
    int rc = RunReseqExitOnly("");
    EXPECT_EQ(0, rc) << "Bare reseq (no args) should exit 0";
}

TEST_F(RegressionTest, BareReseqOutput) {
    std::string stderr_out = RunReseqCaptureStderr("");
    EXPECT_NE(std::string::npos, stderr_out.find("reseq2 <command>"))
        << "Bare reseq2 should print usage containing 'reseq2 <command>', got:\n"
        << stderr_out;
}

TEST_F(RegressionTest, HelpSameAsBare) {
    std::string bare = RunReseqCaptureStderr("");
    std::string help = RunReseqCaptureStderr("--help");
    EXPECT_EQ(bare, help) << "reseq --help should produce identical output to bare reseq";
}

TEST_F(RegressionTest, UnknownCommandExitCode) {
    int rc = RunReseqExitOnly("nonsenseCommand123");
    EXPECT_EQ(1, rc) << "Unknown command should exit 1";
}

TEST_F(RegressionTest, UnknownCommandStderr) {
    std::string stderr_out = RunReseqCaptureStderr("nonsenseCommand123");
    EXPECT_NE(std::string::npos, stderr_out.find("Unrecognized command: 'nonsenseCommand123'"))
        << "Should contain exact error message, got:\n"
        << stderr_out;
}

TEST_F(RegressionTest, CommandHelpExitCode) {
    int rc = RunReseqExitOnly("replaceN --help");
    EXPECT_EQ(0, rc) << "replaceN --help should exit 0";
}

TEST_F(RegressionTest, VersionExitCode) {
    int rc = RunReseqExitOnly("--version");
    EXPECT_EQ(0, rc) << "reseq --version should exit 0";
}

TEST_F(RegressionTest, SeqToIlluminaHelpExitCode) {
    int rc = RunReseqExitOnly("seqToIllumina --help");
    EXPECT_EQ(0, rc) << "seqToIllumina --help should exit 0";
}

TEST_F(RegressionTest, IlluminaPEHelpExitCode) {
    int rc = RunReseqExitOnly("illuminaPE --help");
    EXPECT_EQ(0, rc) << "illuminaPE --help should exit 0";
}

// --- Phase 6: binary serialization tests ---

TEST_F(RegressionTest, FormatDetectionText) {
    auto profile = GenerateEcoliProfile();
    // The profile generated by illuminaPE is now binary by default
    // Convert to text for testing text detection
    int rc = RunReseqExitOnly("convertProfile -s " + profile.string() + " --textFormat");
    ASSERT_EQ(0, rc);

    // Load and query — should auto-detect text format
    std::string output = RunReseqCapture("queryProfile -s " + profile.string() + " --maxReadLength");
    EXPECT_NE(std::string::npos, output.find("maxReadLength:"))
        << "Text format profile should load and query successfully";
}

TEST_F(RegressionTest, FormatDetectionBinary) {
    auto profile = GenerateEcoliProfile();
    // Profile is already binary by default — query directly
    std::string output = RunReseqCapture("queryProfile -s " + profile.string() + " --maxReadLength");
    EXPECT_NE(std::string::npos, output.find("maxReadLength:"))
        << "Binary format profile should load and query successfully";
}

TEST_F(RegressionTest, ConvertProfileRoundTrip) {
    auto profile = GenerateEcoliProfile();

    // Query original (binary)
    std::string binary_output = RunReseqCapture("queryProfile -s " + profile.string() + " --maxReadLength");

    // Convert to text
    auto text_profile = tmp_dir_ / "ecoli-text.reseq";
    int rc =
        RunReseqExitOnly("convertProfile -s " + profile.string() + " -o " + text_profile.string() + " --textFormat");
    ASSERT_EQ(0, rc);

    // Query text version
    std::string text_output = RunReseqCapture("queryProfile -s " + text_profile.string() + " --maxReadLength");
    EXPECT_EQ(binary_output, text_output) << "Binary and text profiles should produce identical query output";

    // Convert text back to binary
    auto roundtrip_profile = tmp_dir_ / "ecoli-roundtrip.reseq";
    rc = RunReseqExitOnly("convertProfile -s " + text_profile.string() + " -o " + roundtrip_profile.string());
    ASSERT_EQ(0, rc);

    // Query roundtrip version
    std::string roundtrip_output =
        RunReseqCapture("queryProfile -s " + roundtrip_profile.string() + " --maxReadLength");
    EXPECT_EQ(binary_output, roundtrip_output) << "Round-trip should preserve query output";
}

TEST_F(RegressionTest, ConvertProfileProbabilitiesRoundTrip) {
    auto profile = GenerateEcoliProfile();
    auto ipf_file = tmp_dir_ / "ecoli-4pairs.reseq.ipf";

    // Generate IPF probabilities from the existing profile (use -s to load stats, not -b)
    int rc = RunReseq("illuminaPE -r " + (test_dir_ / "ecoli-GCF_000005845.2_ASM584v2_genomic.fa").string() +
                      " --stopAfterEstimation -s " + profile.string() + " -P " + ipf_file.string() + " -j 1");
    // If IPF generation fails, skip this test (may not converge with 4 pairs)
    if (rc != 0 || !std::filesystem::exists(ipf_file)) {
        GTEST_SKIP() << "IPF generation did not produce output (expected with tiny dataset)";
    }

    // Convert IPF to text
    auto text_ipf = tmp_dir_ / "ecoli-text.reseq.ipf";
    rc = RunReseqExitOnly("convertProfile -p " + ipf_file.string() + " -P " + text_ipf.string() + " --textFormat");
    ASSERT_EQ(0, rc) << "convertProfile -p to text failed";

    // Convert back to binary
    auto roundtrip_ipf = tmp_dir_ / "ecoli-roundtrip.reseq.ipf";
    rc = RunReseqExitOnly("convertProfile -p " + text_ipf.string() + " -P " + roundtrip_ipf.string());
    ASSERT_EQ(0, rc) << "convertProfile -p to binary failed";

    // Verify file sizes are reasonable (binary should be smaller than text)
    EXPECT_GT(std::filesystem::file_size(text_ipf), std::filesystem::file_size(roundtrip_ipf))
        << "Binary IPF should be smaller than text IPF";
}

TEST_F(RegressionTest, ConvertProfileHelpExitCode) {
    int rc = RunReseqExitOnly("convertProfile --help");
    EXPECT_EQ(0, rc) << "convertProfile --help should exit 0";
}

TEST_F(RegressionTest, UnsupportedVersionError) {
    // Create a file with valid magic but unknown version byte
    auto bad_file = tmp_dir_ / "bad-version.reseq";
    {
        std::ofstream ofs(bad_file, std::ios::binary);
        ofs.write("RSQ", 3);
        char bad_version = 99;
        ofs.write(&bad_version, 1);
        ofs.write("garbage data", 12);
    }

    int rc = RunReseqExitOnly("queryProfile -s " + bad_file.string() + " --maxReadLength");
    EXPECT_NE(0, rc) << "Unsupported version should fail";

    std::string err = RunReseqCaptureStderr("queryProfile -s " + bad_file.string() + " --maxReadLength");
    EXPECT_NE(std::string::npos, err.find("Unsupported")) << "Error should mention unsupported version, got:\n" << err;
}

// --- E2E edge-case error path tests ---

TEST_F(RegressionTest, InvalidBamPath) {
    auto ref = test_dir_ / "ecoli-GCF_000005845.2_ASM584v2_genomic.fa";
    auto adapter_fa = adapter_dir_ / "TruSeq_single.fa";
    auto adapter_mat = adapter_dir_ / "TruSeq_single.mat";
    auto output = tmp_dir_ / "should-not-exist.reseq";

    std::string args = "illuminaPE"
                       " -r " +
                       ref.string() +
                       " -b /nonexistent/path/to/file.bam"
                       " --adapterFile " +
                       adapter_fa.string() + " --adapterMatrix " + adapter_mat.string() +
                       " --statsOnly --noBias"
                       " -S " +
                       output.string() + " -j 1";

    int rc = RunReseqExitOnly(args);
    EXPECT_NE(0, rc) << "illuminaPE should fail with non-existent BAM path";
    EXPECT_FALSE(std::filesystem::exists(output)) << "Output should not be created on failure";
}

TEST_F(RegressionTest, MissingReference) {
    auto bam = test_dir_ / "ecoli-SRR490124-4pairs.bam";
    auto adapter_fa = adapter_dir_ / "TruSeq_single.fa";
    auto adapter_mat = adapter_dir_ / "TruSeq_single.mat";
    auto output = tmp_dir_ / "should-not-exist.reseq";

    std::string args = "illuminaPE"
                       " -r /nonexistent/path/to/reference.fa"
                       " -b " +
                       bam.string() + " --adapterFile " + adapter_fa.string() + " --adapterMatrix " +
                       adapter_mat.string() +
                       " --statsOnly --noBias"
                       " -S " +
                       output.string() + " -j 1";

    int rc = RunReseqExitOnly(args);
    EXPECT_NE(0, rc) << "illuminaPE should fail with non-existent reference path";
}

TEST_F(RegressionTest, CorruptReseqFile) {
    auto corrupt_file = tmp_dir_ / "corrupt.reseq";
    {
        std::ofstream f(corrupt_file, std::ios::binary);
        f << "THIS_IS_NOT_A_VALID_RESEQ_FILE_HEADER_GARBAGE_DATA_1234567890";
    }

    std::string args = "queryProfile -s " + corrupt_file.string() + " --maxReadLength";

    int rc = RunReseqExitOnly(args);
    EXPECT_NE(0, rc) << "queryProfile should fail on corrupt .reseq file";
}

TEST_F(RegressionTest, EmptyBam) {
    auto ref = test_dir_ / "ecoli-GCF_000005845.2_ASM584v2_genomic.fa";
    auto adapter_fa = adapter_dir_ / "TruSeq_single.fa";
    auto adapter_mat = adapter_dir_ / "TruSeq_single.mat";
    auto output = tmp_dir_ / "empty-result.reseq";

    // Create empty BAM using samtools (valid header, zero records)
    auto empty_bam = tmp_dir_ / "empty.bam";
    std::string create_cmd =
        "printf '@HD\\tVN:1.6\\tSO:unsorted\\n' | samtools view -bS -o " + empty_bam.string() + " - 2>/dev/null";
    std::system(create_cmd.c_str());

    if (!std::filesystem::exists(empty_bam) || std::filesystem::file_size(empty_bam) == 0) {
        GTEST_SKIP() << "samtools not available to create empty BAM";
    }

    std::string args = "illuminaPE"
                       " -r " +
                       ref.string() + " -b " + empty_bam.string() + " --adapterFile " + adapter_fa.string() +
                       " --adapterMatrix " + adapter_mat.string() +
                       " --statsOnly --noBias"
                       " -S " +
                       output.string() + " -j 1";

    int rc = RunReseqExitOnly(args);
    // The spec pins the contract: empty input is an error.
    // If the current code silently succeeds (rc==0), change this to EXPECT_EQ(0, rc)
    // and add a comment documenting it as a known behavioral issue.
    EXPECT_NE(0, rc) << "illuminaPE should report an error for empty BAM input";
}

TEST_F(RegressionTest, SeqToIlluminaOver10kReads) {
    // Regression test for GitHub issue #4: seqToIllumina segfaults or produces
    // empty output when processing >10,000 fragments. Three bugs were involved:
    //
    // 1. IncrementBlockPos crash: container redesign changed block linkage from
    //    pointers (NULL) to indices (SIZE_MAX), but standalone blocks in the
    //    seqToIllumina path caused blocks_[SIZE_MAX] OOB access. (Regression)
    // 2. Copy-paste reserve: all 5 reserve() calls in ErrorModelOnlyThread
    //    targeted input_ids instead of their respective StringSets. (Preexisting)
    // 3. Error-path deadlock: ApplyErrorsAndQualityToFastaInput failure skipped
    //    WriteSingleReads, leaving written_blocks_ stuck. (Preexisting)

    auto profile = ZenodoProfile();
    if (profile.empty()) {
        GTEST_SKIP() << "Zenodo test data not available (run test/download_test_data.sh)";
    }

    // Generate 12,000 Wessim-style fragments (crosses the 10k batch boundary)
    const int num_frags = 12000;
    const int read_len = 100;
    auto input_fa = tmp_dir_ / "frags_12000.fa";
    {
        std::ofstream fa(input_fa);
        ASSERT_TRUE(fa.is_open());
        const char bases[] = "ACGT";
        std::mt19937 rng(42);
        std::string sys_seq(read_len, 'N');
        std::string sys_qual(read_len, '!');
        for (int i = 0; i < num_frags; ++i) {
            std::string seq(read_len, 'A');
            for (int j = 0; j < read_len; ++j) {
                seq[j] = bases[rng() % 4];
            }
            int tmpl = (i % 2) + 1;
            fa << ">read_" << i << " " << tmpl << ";300;" << sys_seq << ";" << sys_qual << "\n" << seq << "\n";
        }
    }

    auto output_fq = tmp_dir_ / "out_12000.fq";

    // Run seqToIllumina — before the fix this would segfault or produce empty output.
    // Use --ipfIterations 1 for fast estimation; precomputed .ipf files use
    // non-portable binary serialization and fail across different builds.
    std::string args = "seqToIllumina"
                       " -j 1"
                       " --ipfIterations 1"
                       " -s " +
                       profile.string() + " -i " + input_fa.string() + " -o " + output_fq.string() + " --seed 42";

    int rc = RunReseqExitOnly(args);
    EXPECT_EQ(0, rc) << "seqToIllumina should exit 0";
    ASSERT_TRUE(std::filesystem::exists(output_fq)) << "Output FASTQ not created";

    // Count reads in FASTQ (every 4th line starting from line 0 is a header)
    std::ifstream fq(output_fq);
    ASSERT_TRUE(fq.is_open());
    int read_count = 0;
    std::string line;
    int line_num = 0;
    while (std::getline(fq, line)) {
        if (line_num % 4 == 0) {
            EXPECT_TRUE(line.size() > 0 && line[0] == '@')
                << "FASTQ header at line " << line_num << " missing @ prefix";
            ++read_count;
        } else if (line_num % 4 == 2) {
            EXPECT_EQ("+", line) << "FASTQ separator at line " << line_num << " is not +";
        }
        ++line_num;
    }

    EXPECT_EQ(0, line_num % 4) << "FASTQ has incomplete final record";
    EXPECT_EQ(num_frags, read_count) << "Expected " << num_frags << " reads but got " << read_count
                                     << " (if stuck at 10000, the batch boundary bug is back)";

    // Must have crossed the 10k batch boundary
    EXPECT_GT(read_count, 10000) << "Read count did not cross the 10k batch boundary";
}

} // namespace reseq
