set(TARGET shared_data)

set(SRC
   ${CMAKE_CURRENT_LIST_DIR}/src/shared_data.cpp
)
add_library(${TARGET} ${SRC})

target_include_directories(${TARGET} PUBLIC
   ${CMAKE_CURRENT_LIST_DIR}/inc
   ${CMAKE_SOURCE_DIR}/core/fw_header/inc
)