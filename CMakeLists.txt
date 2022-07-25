project(blink LANGUAGES CXX C ASM)

cmake_minimum_required(VERSION 3.23.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(TARGET blink)

set(CMAKE_EXECUTABLE_SUFFIX ".elf")
set(LINKER_FILE "${CMAKE_CURRENT_LIST_DIR}/core/memory_layout/app_memory_layout.ld")
message(STATUS "Linker Script for ${TARGET}: ${LINKER_FILE}")
set(CMAKE_EXE_LINKER_FLAGS "-lc -lm -lnosys -specs=nano.specs -L${CMAKE_SOURCE_DIR}/core/memory_layout/ -T${LINKER_FILE} -Wl,-Map=${CMAKE_SOURCE_DIR}/build/app_memory_map.map,--cref -Wl,--gc-sections")

# Source files
set(SRC
   ${CMAKE_CURRENT_LIST_DIR}/startup.s
   ${CMAKE_CURRENT_LIST_DIR}/core/main.cpp
   ${CMAKE_CURRENT_LIST_DIR}/core/syscalls.c
   ${CMAKE_CURRENT_LIST_DIR}/core/stm32f7xx_hal_msp.c
   ${CMAKE_CURRENT_LIST_DIR}/core/stm32f7xx_hal_timebase_tim.c
   ${CMAKE_CURRENT_LIST_DIR}/core/interrupts/src/stm32f7xx_it.c
   ${CMAKE_CURRENT_LIST_DIR}/drivers/CMSIS/Device/ST/STM32F7xx/Source/Templates/system_stm32f7xx.c
   ${CMAKE_CURRENT_LIST_DIR}/drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.c
   ${CMAKE_CURRENT_LIST_DIR}/drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.c
   ${CMAKE_CURRENT_LIST_DIR}/drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.c
   ${CMAKE_CURRENT_LIST_DIR}/drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr.c
   ${CMAKE_CURRENT_LIST_DIR}/drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr_ex.c
   ${CMAKE_CURRENT_LIST_DIR}/drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.c
   ${CMAKE_CURRENT_LIST_DIR}/drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc_ex.c
   ${CMAKE_CURRENT_LIST_DIR}/drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim.c
   ${CMAKE_CURRENT_LIST_DIR}/drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim_ex.c
)
# Build the application executable
add_executable(${TARGET} ${SRC})

# Includes
# These need to be cleaned up, I don't think they're all necessary
target_include_directories(${TARGET} PUBLIC 
   ${CMAKE_CURRENT_LIST_DIR}/core/
   ${CMAKE_CURRENT_LIST_DIR}/core/crc32/inc
   ${CMAKE_CURRENT_LIST_DIR}/core/board_defines/inc
   ${CMAKE_CURRENT_LIST_DIR}/core/interrupts/inc
   ${CMAKE_CURRENT_LIST_DIR}/core/memory_layout/inc
   ${CMAKE_CURRENT_LIST_DIR}/drivers/CMSIS/Include
   ${CMAKE_CURRENT_LIST_DIR}/drivers/CMSIS/Device/ST/STM32F7xx/Include
   ${CMAKE_CURRENT_LIST_DIR}/drivers/STM32F7xx_HAL_Driver/Inc
   ${CMAKE_CURRENT_LIST_DIR}/drivers/STM32F7xx_HAL_Driver/Legacy
)

add_compile_definitions(STM32F746xx USE_HAL_DRIVER USE_FULL_ASSERT)

# add_subdirectory for bootloader?
add_subdirectory(core/bootloader)

# add_subdirectory _after_ actions, so that subdirs inherit variables and such
add_subdirectory(core/assert)
add_subdirectory(core/crc)
add_subdirectory(core/flash)
add_subdirectory(core/fw_image)
add_subdirectory(core/fw_update)
add_subdirectory(core/lcd)
add_subdirectory(core/shared_data)
add_subdirectory(core/uart)
add_subdirectory(core/usb_host)
target_link_libraries(${TARGET} assert crc flash fw_image fw_update lcd shared_data uart usb_host)
# Strip out the debug sections from the .elf for breaking up into the binary, but leave the old .elf for debugging
add_custom_command(TARGET ${TARGET} POST_BUILD COMMAND arm-none-eabi-objcopy -g ${CMAKE_CURRENT_LIST_DIR}/build/${TARGET}.elf ${CMAKE_CURRENT_LIST_DIR}/build/${TARGET}_stripped.elf)
add_custom_command(TARGET ${TARGET} POST_BUILD COMMAND arm-none-eabi-objdump -h ${CMAKE_CURRENT_LIST_DIR}/build/${TARGET}_stripped.elf > ${CMAKE_CURRENT_LIST_DIR}/build/sections.txt)
add_custom_command(TARGET ${TARGET} POST_BUILD COMMAND arm-none-eabi-objdump -S ${CMAKE_CURRENT_LIST_DIR}/build/${TARGET}.elf > ${CMAKE_CURRENT_LIST_DIR}/build/${TARGET}.txt)
add_custom_command(TARGET ${TARGET} POST_BUILD COMMAND arm-none-eabi-size ${CMAKE_CURRENT_LIST_DIR}/build/${TARGET}.elf)