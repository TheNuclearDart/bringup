set(TARGET fw_tasks)
message(STATUS "Building ${TARGET} library")

set(SRC
   ${CMAKE_CURRENT_LIST_DIR}/src/main_task.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/input_task.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/lcd_task.cpp
   ${CMAKE_CURRENT_LIST_DIR}/src/uart_task.cpp
)
add_library(${TARGET} ${SRC})

target_include_directories(${TARGET} PUBLIC
   ${CMAKE_CURRENT_LIST_DIR}/inc
   ${CMAKE_SOURCE_DIR}/core
   ${CMAKE_SOURCE_DIR}/core/assert/inc
   ${CMAKE_SOURCE_DIR}/core/board_defines/inc
   ${CMAKE_SOURCE_DIR}/core/crc/inc
   ${CMAKE_SOURCE_DIR}/core/fw_header/inc
   ${CMAKE_SOURCE_DIR}/core/fw_image/inc
   ${CMAKE_SOURCE_DIR}/core/lcd/inc
   ${CMAKE_SOURCE_DIR}/core/memory_layout/inc
   ${CMAKE_SOURCE_DIR}/core/uart/inc
   ${CMAKE_SOURCE_DIR}/drivers/CMSIS/Include
   ${CMAKE_SOURCE_DIR}/drivers/CMSIS/Device/ST/STM32F7xx/Include
   ${CMAKE_SOURCE_DIR}/drivers/STM32F7xx_HAL_Driver/Inc
   ${CMAKE_SOURCE_DIR}/drivers/STM32F7xx_HAL_Driver/Legacy
   # FreeRTOS
   ${CMAKE_SOURCE_DIR}/third_party/FreeRTOS/Source/include
   ${CMAKE_SOURCE_DIR}/third_party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1
   # LVGL
   ${CMAKE_SOURCE_DIR}/third_party/lvgl
)
target_link_libraries(${TARGET} assert crc lcd uart fw_image fw_update print)