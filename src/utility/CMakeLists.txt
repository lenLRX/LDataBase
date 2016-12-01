aux_source_directory(. UTILITY_SOURCE)

add_library(utility_static STATIC ${UTILITY_SOURCE})
SET_TARGET_PROPERTIES(utility_static PROPERTIES OUTPUT_NAME "utility")  
SET_TARGET_PROPERTIES(utility_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)