set(TARGET fw_header)
message(STATUS "Building ${TARGET} library")

set(SRC
   ${CMAKE_CURRENT_LIST_DIR}/src/fw_header.cpp
)
add_library(${TARGET} ${SRC})

target_include_directories(${TARGET} PUBLIC
   ${CMAKE_CURRENT_LIST_DIR}/inc
)