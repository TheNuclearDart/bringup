set(TARGET print)
message(STATUS "Building ${TARGET} library")

set(SRC
   ${CMAKE_CURRENT_LIST_DIR}/src/print.cpp
)
add_library(${TARGET} ${SRC})

target_include_directories(${TARGET} PUBLIC
   ${CMAKE_CURRENT_LIST_DIR}/inc
   ${CMAKE_SOURCE_DIR}/core/
   ${CMAKE_SOURCE_DIR}/core/board_defines/inc
   ${CMAKE_SOURCE_DIR}/core/fw_header/inc
   ${CMAKE_SOURCE_DIR}/core/fw_tasks/inc
   ${CMAKE_SOURCE_DIR}/core/memory_layout/inc
   ${CMAKE_SOURCE_DIR}/core/shared_data/inc
   ${CMAKE_SOURCE_DIR}/drivers/CMSIS/Include
   ${CMAKE_SOURCE_DIR}/drivers/CMSIS/Device/ST/STM32F7xx/Include
   ${CMAKE_SOURCE_DIR}/drivers/STM32F7xx_HAL_Driver/Inc
   ${CMAKE_SOURCE_DIR}/drivers/STM32F7xx_HAL_Driver/Legacy
   ${CMAKE_SOURCE_DIR}/third_party/FreeRTOS/Source/include
   ${CMAKE_SOURCE_DIR}/third_party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1
)
target_link_libraries(${TARGET} shared_data)