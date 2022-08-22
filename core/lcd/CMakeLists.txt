set(TARGET lcd)

set(SRC
   ${CMAKE_CURRENT_LIST_DIR}/src/lcd.cpp
   ${CMAKE_SOURCE_DIR}/drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_ltdc.c
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