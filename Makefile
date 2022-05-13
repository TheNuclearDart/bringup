#########################################################
# This is my very basic makefile. 
# Should replace this with CMake when it gets more complex
#########################################################

# Basic config - target, optimization, build output dir
TARGET = blink

DEBUG = 1
OPT = -Og

BUILD_DIR = build

# cpu
CPU = -mcpu=cortex-m7

# fpu
FPU = -mfpu=fpv5-sp-d16

# float-abi
FLOAT-ABI = -mfloat-abi=hard

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# Build tools
# g++ instead of gcc for c++
#prefix to help shorten names
PREFIX = arm-none-eabi-
ifdef GCC_PATH #Can be defined in scripts to use a different gcc
CC = $(GCC_PATH)/$(PREFIX)g++
CXX = $(GCC_PATH)/$(PREFIX)g++
AS = $(GCC_PATH)/$(PREFIX)g++ -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)g++
CXX = $(PREFIX)g++
AS = $(PREFIX)g++ -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

# C Defines
C_DEFS = \
-DSTM32F746xx \
-DUSE_HAL_DRIVER \

# C Includes
C_INCLUDES = \
-ICore/board_defines/inc \
-ICore/interrupts/inc \
-ICore/print/inc \
-ICore/usb_host/inc \
-IDrivers/CMSIS/Include \
-IDrivers/CMSIS/Device/ST/STM32F7xx/Include \
-IDrivers/STM32F7xx_HAL_Driver/Inc \
-IDrivers/STM32F7xx_HAL_Driver/Inc/Legacy \
-IThird_party/st_usb_host/Core/Inc \
-IThird_party/st_usb_host/Class/CDC/Inc \

# C Source
C_SOURCES = \
core/interrupts/src/stm32f7xx_it.c \
core/stm32f7xx_hal_msp.c \
core/stm32f7xx_hal_timebase_tim.c \
drivers/CMSIS/Device/ST/STM32F7xx/Source/Templates/system_stm32f7xx.c \
drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.c \
drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.c \
drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.c \
drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_hcd.c \
drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr.c \
drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr_ex.c \
drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.c \
drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc_ex.c \
drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim.c \
drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim_ex.c \
drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_uart.c \
third_party/st_usb_host/Core/Src/usbh_core.c \
third_party/st_usb_host/Core/Src/usbh_ctlreq.c \
third_party/st_usb_host/Core/Src/usbh_ioreq.c \
third_party/st_usb_host/Core/Src/usbh_pipes.c \

# C++ Includes
CXX_INCLUDES = \
-ICore/board_defines/inc \
-ICore/print/inc \
-ICore/usb_host/inc \
-IDrivers/CMSIS/Include \
-IDrivers/CMSIS/Device/ST/STM32F7xx/Include \
-IDrivers/STM32F7xx_HAL_Driver/Inc \
-IDrivers/STM32F7xx_HAL_Driver/Inc/Legacy \
-IThird_party/st_usb_host/Core/Inc \
-IThird_party/st_usb_host/Class/CDC/Inc \

# C++ Source
CXX_SOURCES = \
core/main.cpp \
core/print/src/print.cpp \
core/usb_host/src/usb_host.cpp \

# ASM Source
ASM_SOURCES = \
startup.s

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections -fno-exceptions -fno-rtti
CXXFLAGS = $(CFLAGS)

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
CXXFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"
CXXFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = memory_layout.ld

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(CXX_SOURCES:.cpp=.o)))
vpath %.cpp $(sort $(dir $(CXX_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.cpp Makefile | $(BUILD_DIR) 
	$(CXX) -c $(CXXFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.cpp=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)
  
#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)