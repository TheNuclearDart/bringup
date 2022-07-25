set(TARGET fw_image)
message(STATUS "Building ${TARGET} library")

set(SRC
   ${CMAKE_CURRENT_LIST_DIR}/src/fw_image.cpp
)
add_library(${TARGET} ${SRC})

target_include_directories(${TARGET} PUBLIC
   ${CMAKE_CURRENT_LIST_DIR}/inc
   ${CMAKE_SOURCE_DIR}/core/fw_header/inc
   ${CMAKE_SOURCE_DIR}/core/memory_layout/inc
   ${CMAKE_SOURCE_DIR}/core/shared_data/inc
   ${CMAKE_SOURCE_DIR}/drivers/CMSIS/Include
   ${CMAKE_SOURCE_DIR}/drivers/CMSIS/Device/ST/STM32F7xx/Include
   ${CMAKE_SOURCE_DIR}/drivers/STM32F7xx_HAL_Driver/Inc
   ${CMAKE_SOURCE_DIR}/drivers/STM32F7xx_HAL_Driver/Legacy
)
target_link_libraries(${TARGET} shared_data)