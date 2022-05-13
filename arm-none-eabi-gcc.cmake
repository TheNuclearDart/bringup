set(CMAKE_SYSTEM_NAME arm)


set(PREFIX arm-none-eabi-)

set(CMAKE_C_COMPILER ${PREFIX}gcc)
message(STATUS "C Compiler: ${CMAKE_C_COMPILER}")

set(CMAKE_CXX_COMPILER ${PREFIX}g++)
message(STATUS "CXX Compiler: ${CMAKE_CXX_COMPILER}")

set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER} -x assembler-with-cpp)
message(STATUS "ASM Compiler: ${CMAKE_ASM_COMPILER}")

set(CMAKE_OBJCOPY ${PREFIX}objcopy)
message(STATUS "Objcopy: ${CMAKE_OJBCOPY}")

set(CMAKE_SIZE_UTIL ${PREFIX}size)
message(STATUS "Size Util: ${CMAKE_SIZE_UTIL}")

set(CPU "-mcpu=cortex-m7")
set(FPU "-mfpu=fpv5-sp-d16")
set(FLOAT_ABI "-mfloat-abi=hard")
string(JOIN " " MCU ${CPU} "-mthumb" ${FPU} ${FLOAT_ABI})
message(STATUS "MCU: ${MCU}")

set(OPT "-0g")
set(DEBUG True)

string(JOIN " " ASFLAGS ${MCU} ${OPT} "-Wall -fdata-sections -ffunction-sections")
string(JOIN " " CFLAGS ${MCU} ${OPT} "-Wall -fdata-sections -ffuntion-sections -fno-exceptions -no-rtti")
set(CXXFLAGS ${CFLAGS})

if(DEBUG EQUAL True)
   string(APPEND CFLAGS "-g -gdwarf-2")
   string(APPEND CXXFLAGS "-g -gdwarf-2")
endif()

string(JOIN " " CMAKE_ASM_FLAGS_DEBUG_INIT ${ASFLAGS})
string(JOIN " " CMAKE_C_FLAGS_DEBUG_INIT ${CFLAGS})
string(JOIN " " CMAKE_CXX_FLAGS_DEBUG_INIT ${CXXFLAGS})
message(STATUS "C Flags: ${CMAKE_CXX_FLAGS_DEBUG_INIT}")

#Might be bad
set(CMAKE_C_COMPILER_FORCED TRUE)
set(CMAKE_CXX_COMPILER_FORCED TRUE)
