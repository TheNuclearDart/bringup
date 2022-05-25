project(blink LANGUAGES CXX C ASM)

cmake_minimum_required(VERSION 3.23.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(TARGET blink)

set(CMAKE_EXECUTABLE_SUFFIX ".elf")
set(LINKER_FILE "${CMAKE_CURRENT_LIST_DIR}/memory_layout.ld")
message(STATUS "Linker Script: ${LINKER_FILE}")
set(CMAKE_EXE_LINKER_FLAGS "-lc -lm -lnosys -specs=nano.specs -T${LINKER_FILE} -Wl,-Map=${CMAKE_SOURCE_DIR}/build/memory_map.map,--cref -Wl,--gc-sections")

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
add_executable(${TARGET} ${SRC})

# Includes
target_include_directories(blink PUBLIC 
   ${CMAKE_CURRENT_LIST_DIR}/core/
   ${CMAKE_CURRENT_LIST_DIR}/core/board_defines/inc
   ${CMAKE_CURRENT_LIST_DIR}/core/interrupts/inc
   ${CMAKE_CURRENT_LIST_DIR}/drivers/CMSIS/Include
   ${CMAKE_CURRENT_LIST_DIR}/drivers/CMSIS/Device/ST/STM32F7xx/Include
   ${CMAKE_CURRENT_LIST_DIR}/drivers/STM32F7xx_HAL_Driver/Inc
   ${CMAKE_CURRENT_LIST_DIR}/drivers/STM32F7xx_HAL_Driver/Legacy
)

add_compile_definitions(STM32F746xx USE_HAL_DRIVER)

# add_subdirectory _after_ actions, so that subdirs inherit variables and such
add_subdirectory(core/print)
add_subdirectory(core/usb_host)
target_link_libraries(${TARGET} print usb_host)
add_custom_command(TARGET ${TARGET} POST_BUILD COMMAND arm-none-eabi-size ${CMAKE_CURRENT_LIST_DIR}/build/${TARGET}.elf)