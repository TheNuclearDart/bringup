set(TARGET crc32)
message(STATUS "Building ${TARGET} library")

set(SRC
   ${CMAKE_CURRENT_LIST_DIR}/src/crc32.cpp
   ${CMAKE_SOURCE_DIR}/drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_crc.c
   ${CMAKE_SOURCE_DIR}/drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_crc_ex.c
)
add_library(${TARGET} ${SRC})

target_include_directories(${TARGET} PUBLIC
   ${CMAKE_CURRENT_LIST_DIR}/inc
   ${CMAKE_SOURCE_DIR}/core/board_defines/inc
   ${CMAKE_SOURCE_DIR}/drivers/CMSIS/Include
   ${CMAKE_SOURCE_DIR}/drivers/CMSIS/Device/ST/STM32F7xx/Include
   ${CMAKE_SOURCE_DIR}/drivers/STM32F7xx_HAL_Driver/Inc
   ${CMAKE_SOURCE_DIR}/drivers/STM32F7xx_HAL_Driver/Legacy
)
target_link_libraries(${TARGET} assert)