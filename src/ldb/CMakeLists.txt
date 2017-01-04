subdirs(experiments)

set(LDB_SOURCE 
./file/FileManager.cpp)

add_library(ldb_static STATIC ${LDB_SOURCE})
SET_TARGET_PROPERTIES(ldb_static PROPERTIES OUTPUT_NAME "ldb")  
SET_TARGET_PROPERTIES(ldb_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)