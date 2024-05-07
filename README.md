Custom STM32F746NG bringup using some CubeMX generated code, but mostly custom.

*Custom build system using CMake and ARM-GCC to build both a standalone application image as well as a full image including a bootloader and application. Uses python to construct a custom header for each image section, so that the bootloader knows where they should be loaded.

*Custom bootloader that initializes SDRAM and loads one of two redundant application images. As images are updated, the bootloader looks at a header to determine which image is active, if it cannot load that image, it loads the other, previous image.

*Application is built on FreeRTOS, with multiple tasks communicating through message queues. Includes a custom message based UART print solution so that each task may print without overrunning/garbling other tasks' prints.

*Uses Xmodem/UART for updating the application image. The image is written to one of two regions/slots in the onboard FLASH (eventually onboard EEPROM), with the update process alternating between each image, so that there is always one good image.

*The bootloader is only updated when a full image (bootloader + application) is downloaded using STLink/JLink.

Required tools:
  
  *make
 
  *CMake
  
  *arm-none-eabi-gcc
