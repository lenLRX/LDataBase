aux_source_directory(. TEST_SOURCE)

add_executable(runtest ${TEST_SOURCE})

TARGET_LINK_LIBRARIES(runtest log_static utility_static ldb_static)