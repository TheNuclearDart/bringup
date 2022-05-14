set(CMAKE_SYSTEM_NAME Generic)

set(PREFIX arm-none-eabi-)

set(CMAKE_C_COMPILER ${PREFIX}gcc)
message(STATUS "C Compiler: ${CMAKE_C_COMPILER}")

set(CMAKE_CXX_COMPILER ${PREFIX}g++)
message(STATUS "CXX Compiler: ${CMAKE_CXX_COMPILER}")

set(CMAKE_ASM_COMPILER "${CMAKE_C_COMPILER}")
message(STATUS "ASM Compiler: ${CMAKE_ASM_COMPILER}")

set(CMAKE_OBJCOPY ${PREFIX}objcopy)
message(STATUS "Objcopy: ${CMAKE_OBJCOPY}")

set(CMAKE_SIZE_UTIL ${PREFIX}size)
message(STATUS "Size Util: ${CMAKE_SIZE_UTIL}")

set(CPU "-mcpu=cortex-m7")
set(FPU "-mfpu=fpv5-sp-d16")
set(FLOAT_ABI "-mfloat-abi=hard")
string(JOIN " " MCU ${CPU} "-mthumb" ${FPU} ${FLOAT_ABI})
message(STATUS "MCU: ${MCU}")

set(OPT "-Og")

string(JOIN " " ASFLAGS " -x assembler-with-cpp" ${MCU} ${OPT} "-Wall -fdata-sections -ffunction-sections")
string(JOIN " " CFLAGS ${MCU} ${OPT} "-Wall -fdata-sections -ffunction-sections -fno-exceptions")
string(JOIN " " CXXFLAGS ${CFLAGS} "-fno-rtti")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
   message(STATUS "Debug build")
   string(APPEND CFLAGS " -g -gdwarf-2")
   string(APPEND CXXFLAGS " -g -gdwarf-2")
   string(APPEND ASFLAGS " -g -gdwarf-2")
endif()

string(JOIN " " CMAKE_ASM_FLAGS_DEBUG ${ASFLAGS})
string(JOIN " " CMAKE_C_FLAGS_DEBUG ${CFLAGS})
string(JOIN " " CMAKE_CXX_FLAGS_DEBUG ${CXXFLAGS})
message(STATUS "C Flags: ${CMAKE_CXX_FLAGS_DEBUG}")

#Might be bad
set(CMAKE_C_COMPILER_FORCED TRUE)
set(CMAKE_CXX_COMPILER_FORCED TRUE)
