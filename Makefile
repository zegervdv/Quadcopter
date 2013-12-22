# Put your source files here (or *.c, etc)
SRCS  = Utilities/STM32F3_Discovery/stm32f3_discovery.c
SRCS += Libraries/STM32F30x_StdPeriph_Driver/src/stm32f30x_rcc.c
SRCS += Libraries/STM32F30x_StdPeriph_Driver/src/stm32f30x_gpio.c
SRCS += Libraries/STM32F30x_StdPeriph_Driver/src/stm32f30x_usart.c
SRCS += Libraries/CMSIS/Device/ST/STM32F30x/Source/Templates/system_stm32f30x.c
SRCS += Libraries/STM32F30x_StdPeriph_Driver/src/stm32f30x_syscfg.c
SRCS += Libraries/STM32F30x_StdPeriph_Driver/src/stm32f30x_exti.c
SRCS += Libraries/STM32F30x_StdPeriph_Driver/src/stm32f30x_misc.c
SRCS += src/main.c
SRCS += src/newlib_stubs.c
SRCS += src/stm32f30x_it.c

# Binaries will be generated with this name (.elf, .bin, .hex, etc)
PROJ_NAME=quadrocopter

# Put your STM32F4 library code directory here
# STM_COMMON=../STM32F4-Discovery_FW_V1.1.0

# Normally you shouldn't need to change anything below this line!
#######################################################################################

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
DEFINES = -DSTM32F3XX \
					-DUSE_STDPERIPH_DRIVER

CFLAGS  = -g -O2 -Wall -Tstm32_flash.ld
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# CFLAGS += --disable-newlib-supplied-syscalls
CFLAGS += -Isrc
CFLAGS += $(DEFINES)

# Include files from STM libraries
CFLAGS += -IUtilities/STM32F3_Discovery
CFLAGS += -ILibraries/CMSIS/Include -ILibraries/CMSIS/Device/ST/STM32F30x/Include
CFLAGS += -Iinc
CFLAGS += -ILibraries/STM32F30x_StdPeriph_Driver/inc
CFLAGS += -ILibraries/STM32_USB-FS-Device_Driver/inc

# add startup file to build
SRCS += Libraries/CMSIS/Device/ST/STM32F30x/Source/Templates/TrueSTUDIO/startup_stm32f30x.s
OBJS = $(SRCS:.c=.o)


.PHONY: proj

all: proj

proj: $(PROJ_NAME).elf

$(PROJ_NAME).elf: $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@
	$(OBJCOPY) -O ihex $(PROJ_NAME).elf $(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin

clean:
	rm -f *.o $(PROJ_NAME).elf $(PROJ_NAME).hex $(PROJ_NAME).bin

# Flash the STM32F4
burn: proj
	st-flash write $(PROJ_NAME).bin 0x8000000
