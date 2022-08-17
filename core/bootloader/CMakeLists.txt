set(TARGET bootloader)
message(STATUS "Building ${TARGET}")

set(LINKER_FILE "${CMAKE_SOURCE_DIR}/core/memory_layout/bl_memory_layout.ld")
message(STATUS "Linker Script for ${TARGET}: ${LINKER_FILE}")
message("CMake Linker Flags: ${CMAKE_EXE_LINKER_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "-lc -lm -lnosys -specs=nano.specs -L${CMAKE_SOURCE_DIR}/core/memory_layout/ -T${LINKER_FILE} -Wl,-Map=${CMAKE_SOURCE_DIR}/build/bl_memory_map.map,--cref -Wl,--gc-sections")
add_compile_definitions(STM32F746xx USE_HAL_DRIVER USE_FULL_ASSERT BOOTLOADER_BUILD SDRAM_TEST) # needed?

set(SRC
   ${CMAKE_SOURCE_DIR}/core/bootloader/src/bl_startup.s
   ${CMAKE_SOURCE_DIR}/core/bootloader/src/bootloader.cpp
   ${CMAKE_SOURCE_DIR}/core/bootloader/src/sdram_init.cpp
   ${CMAKE_SOURCE_DIR}/core/syscalls.c
   ${CMAKE_SOURCE_DIR}/core/stm32f7xx_hal_msp.c
   ${CMAKE_SOURCE_DIR}/core/stm32f7xx_hal_timebase_tim.c
   ${CMAKE_SOURCE_DIR}/core/interrupts/src/stm32f7xx_it.c
   ${CMAKE_SOURCE_DIR}/drivers/CMSIS/Device/ST/STM32F7xx/Source/Templates/system_stm32f7xx.c
   ${CMAKE_SOURCE_DIR}/drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.c
   ${CMAKE_SOURCE_DIR}/drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.c
   ${CMAKE_SOURCE_DIR}/drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.c
   ${CMAKE_SOURCE_DIR}/drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr.c
   ${CMAKE_SOURCE_DIR}/drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr_ex.c
   ${CMAKE_SOURCE_DIR}/drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.c
   ${CMAKE_SOURCE_DIR}/drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc_ex.c
   ${CMAKE_SOURCE_DIR}/drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sdram.c
   ${CMAKE_SOURCE_DIR}/drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim.c
   ${CMAKE_SOURCE_DIR}/drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim_ex.c
   ${CMAKE_SOURCE_DIR}/drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_fmc.c
)
add_executable(${TARGET} ${SRC})

# Includes
target_include_directories(${TARGET} PUBLIC
   ${CMAKE_CURRENT_LIST_DIR}/inc
   ${CMAKE_SOURCE_DIR}/core/
   ${CMAKE_SOURCE_DIR}/core/board_defines/inc
   ${CMAKE_SOURCE_DIR}/core/interrupts/inc
   ${CMAKE_SOURCE_DIR}/core/memory_layout/inc
   ${CMAKE_SOURCE_DIR}/drivers/CMSIS/Include
   ${CMAKE_SOURCE_DIR}/drivers/CMSIS/Device/ST/STM32F7xx/Include
   ${CMAKE_SOURCE_DIR}/drivers/STM32F7xx_HAL_Driver/Inc
   ${CMAKE_SOURCE_DIR}/drivers/STM32F7xx_HAL_Driver/Legacy
)

target_link_libraries(${TARGET} assert crc fw_image shared_data uart)
# Don't forget to adjust "--pad-to" when changing location of Bootloader binary! If there's a way to set this globally, need to do that instead
add_custom_command(TARGET ${TARGET} POST_BUILD COMMAND arm-none-eabi-objcopy --pad-to=0x08010000 --gap-fill=0xFF -O binary ${CMAKE_CURRENT_BINARY_DIR}/${TARGET}.elf ${CMAKE_CURRENT_BINARY_DIR}/${TARGET}.bin)
add_custom_command(TARGET ${TARGET} POST_BUILD COMMAND arm-none-eabi-objdump -S ${CMAKE_CURRENT_BINARY_DIR}/${TARGET}.elf > ${CMAKE_CURRENT_BINARY_DIR}/${TARGET}.txt)
add_custom_command(TARGET ${TARGET} POST_BUILD COMMAND arm-none-eabi-size ${CMAKE_CURRENT_BINARY_DIR}/${TARGET}.elf)