set(PRIVATE_COMPILE_OPTIONS -Wall -Wextra -Wno-sign-compare -Wno-implicit-fallthrough -fPIC)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  list(APPEND PRIVATE_COMPILE_OPTIONS -fext-numeric-literals -Wno-deprecated-declarations)
endif()

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# Core library -- all production source except main.cpp and *Test.*
add_library(reseq2_lib STATIC
  AdapterStats.cpp
  archive_format.cpp
  BamIngestionEngine.cpp
  CoverageStats.cpp
  DataStats.cpp
  DataStatsInterface.cpp
  ErrorStats.cpp
  FragmentDistributionStats.cpp
  FragmentDuplicationStats.cpp
  ProbabilityEstimates.cpp
  QualityStats.cpp
  ReadSequenceStats.cpp
  Reference.cpp
  Simulator.cpp
  Surrounding.cpp
  TileStats.cpp
)
target_include_directories(reseq2_lib PUBLIC
  "${PROJECT_BINARY_DIR}"
  "${PROJECT_SOURCE_DIR}"
  "${PROJECT_SOURCE_DIR}/reseq"
)
target_link_libraries(reseq2_lib PUBLIC
  skewer_matrix nlopt reseq_seqan
  ${Boost_LIBRARIES} ZLIB::ZLIB BZip2::BZip2 Threads::Threads rt
)
target_compile_options(reseq2_lib PRIVATE ${PRIVATE_COMPILE_OPTIONS})

# Production executable
add_executable(reseq2 main.cpp)
target_sources(reseq2 PRIVATE cli/cli_common.cpp cli/convert_profile.cpp cli/illumina_pe.cpp cli/query_profile.cpp cli/replace_n.cpp cli/seq_to_illumina.cpp)
target_link_libraries(reseq2 PRIVATE reseq2_lib)
target_compile_options(reseq2 PRIVATE ${PRIVATE_COMPILE_OPTIONS})

install(TARGETS reseq2
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)


if(RESEQ_BUILD_TESTS)
  add_executable(reseq2_test
    test_main.cpp
    AdapterStatsTest.cpp CoverageStatsTest.cpp DataStatsTest.cpp
    ErrorStatsTest.cpp FragmentDistributionStatsTest.cpp
    FragmentDuplicationStatsTest.cpp ProbabilityEstimatesTest.cpp
    QualityStatsTest.cpp ReadSequenceStatsTest.cpp ReferenceTest.cpp SeqQualityStatsTest.cpp
    SimulatorTest.cpp SurroundingTest.cpp TileStatsTest.cpp
    utilitiesTest.cpp VectTest.cpp
    RegressionTest.cpp
  )
  target_link_libraries(reseq2_test PRIVATE reseq2_lib GTest::gtest)
  target_compile_options(reseq2_test PRIVATE ${PRIVATE_COMPILE_OPTIONS})
  # Run all tests as a single invocation — tests have inter-test dependencies
  # (shared Register() state) that require running together, not individually.
  add_test(NAME reseq2_tests COMMAND reseq2_test)

  if(CODE_COVERAGE)
    target_compile_options(reseq2_test PRIVATE --coverage -fprofile-update=atomic)
    target_link_options(reseq2_test PRIVATE --coverage)
  endif()
endif()

if(CODE_COVERAGE)
  target_compile_options(reseq2_lib PRIVATE --coverage -fprofile-update=atomic)
  target_link_options(reseq2_lib PRIVATE --coverage)
  target_compile_options(reseq2 PRIVATE --coverage -fprofile-update=atomic)
  target_link_options(reseq2 PRIVATE --coverage)
endif()
