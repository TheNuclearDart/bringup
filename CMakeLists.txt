project(blink)

cmake_minimum_required(VERSION 3.23.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_EXECUTABLE_SUFFIX ".elf")
set(LINKER_FILE "${CMAKE_CURRENT_LIST_DIR}/memory_layout.ld")
message(STATUS "Linker_File: ${LINKER_FILE}")
set(CMAKE_EXE_LINKER_FLAGS_INIT "-lc -lm -lnosys -specs=nano.specs -T ${LINKER_FILE} -Wl,-Map=./build/memory_map.map,--cref -Wl,--gc-sections")

message(STATUS "TEST")