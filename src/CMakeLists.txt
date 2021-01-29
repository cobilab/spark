cmake_minimum_required(VERSION 2.8.4)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lpthread -lm -O3 -march=native -mtune=native ")
IF(UNIX)
 link_libraries(m)
ENDIF(UNIX)

project (SPARK)
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
SET(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
SET(CMAKE_BUILD_TYPE Release)

add_executable (SPARK spark.c mem.c msg.c args.c rand.c fastmem.c misc.c buffer.c context.c pmodels.c array.c tolerant.c hash.c tm.c defs.h)

TARGET_LINK_LIBRARIES(SPARK pthread)
