set(TARGET usb_host)
message(STATUS "Building ${TARGET} library")
set(SRC
   ${CMAKE_CURRENT_LIST_DIR}/src/usbh_conf.c
   ${CMAKE_CURRENT_LIST_DIR}/src/usbh_platform.c
   ${CMAKE_CURRENT_LIST_DIR}/src/usb_host.cpp
   ${CMAKE_SOURCE_DIR}/drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_hcd.c
   ${CMAKE_SOURCE_DIR}/drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_usb.c
   ${CMAKE_SOURCE_DIR}/third_party/st_usb_host/Class/CDC/Src/usbh_cdc.c
   ${CMAKE_SOURCE_DIR}/third_party/st_usb_host/Core/Src/usbh_core.c
   ${CMAKE_SOURCE_DIR}/third_party/st_usb_host/Core/Src/usbh_ctlreq.c
   ${CMAKE_SOURCE_DIR}/third_party/st_usb_host/Core/Src/usbh_ioreq.c
   ${CMAKE_SOURCE_DIR}/third_party/st_usb_host/Core/Src/usbh_pipes.c
)
add_library(${TARGET} ${SRC})

target_include_directories(${TARGET} PUBLIC
   ${CMAKE_CURRENT_LIST_DIR}/inc
   ${CMAKE_SOURCE_DIR}/core/board_defines/inc
   ${CMAKE_SOURCE_DIR}/drivers/CMSIS/Include
   ${CMAKE_SOURCE_DIR}/drivers/CMSIS/Device/ST/STM32F7xx/Include
   ${CMAKE_SOURCE_DIR}/drivers/STM32F7xx_HAL_Driver/Inc
   ${CMAKE_SOURCE_DIR}/third_party/st_usb_host/Core/Inc
   ${CMAKE_SOURCE_DIR}/third_party/st_usb_host/Class/CDC/Inc
)
target_link_libraries(${TARGET} assert)