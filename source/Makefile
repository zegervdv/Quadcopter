
# Put your stlink folder here so make burn will work.
# STLINK=~/stlink.git

# Put your source files here (or *.c, etc)
SRCS = main.c
SRCS += system_stm32f30x.c
SRCS += stm32f30x_it.c
SRCS += system.c
SRCS += bluetooth.c
SRCS += sensors.c
SRCS += controls.c
SRCS += motors.c

# Drivers
SRCS += stm32f30x_rcc.c\
				stm32f30x_exti.c\
				stm32f30x_gpio.c\
				stm32f30x_i2c.c\
				stm32f30x_spi.c\
				stm32f30x_it.c\
				stm32f30x_rtc.c\
				stm32f30x_misc.c\
				stm32f30x_usart.c\
				stm32f30x_syscfg.c\
				stm32f30x_tim.c\
				stm32f30x_crc.c\
				stm32f30x_iwdg.c\
				stm32f30x_adc.c\
				stm32f3_discovery.c\
				stm32f3_discovery_l3gd20.c\
				stm32f3_discovery_lsm303dlhc.c

# Binaries will be generated with this name (.elf, .bin, .hex, etc)
PROJ_NAME=drone

# Put your STM32F4 library code directory here
STM_COMMON=../STM32F3-Discovery_FW_V1.1.0

# Normally you shouldn't need to change anything below this line!
#######################################################################################

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
SIZE=arm-none-eabi-size

CFLAGS  =  -Wall -Tstm32_flash.ld -Wl,--gc-sections
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16 -fsingle-precision-constant
CFLAGS += -I.

CFLAGS += -fno-strict-aliasing
CFLAGS += -DUSE_STDPERIPH_DRIVER
CFLAGS += -DSTM32F3XX
# CFLAGS += -DSTM32F405VG
CFLAGS += -DARM_MATH_CM4
CFLAGS += -DARM_MATH_ROUNDING
CFLAGS += -D__FPU_PRESENT
CFLAGS += -DARM_MATH_MATRIX_CHECK
CFLAGS += -DDELAY_TIME=100
CFLAGS += -DUSE_DEFAULT_TIMEOUT_CALLBACK

CFLAGS += -L/usr/arm-none-eabi/lib/thumb/cortex-m4/float-abi-hard/fpuv4-sp-d16
CFLAGS += --specs=nosys.specs
LIBS += /usr/arm-none-eabi/lib/thumb/cortex-m4/float-abi-hard/fpuv4-sp-d16/libc.a /usr/arm-none-eabi/lib/thumb/cortex-m4/float-abi-hard/fpuv4-sp-d16/libm.a

vpath %.c \
			$(STM_COMMON)/Libraries/STM32F30x_StdPeriph_Driver/src \
			$(STM_COMMON)/Libraries/STM32_USB-FS-Device_Driver/src \
			$(STM_COMMON)/Utilities/STM32F3_Discovery

# Include files from STM libraries
CFLAGS += -I$(STM_COMMON)/Utilities/STM32F3_Discovery
CFLAGS += -I$(STM_COMMON)/Libraries/CMSIS/Include -I$(STM_COMMON)/Libraries/CMSIS/Device/ST/STM32F30x/Include
CFLAGS += -I$(STM_COMMON)/Libraries/STM32F30x_StdPeriph_Driver/inc
CFLAGS += -I$(STM_COMMON)/Libraries/STM32_USB-FS-Device_Driver/inc

# add startup file to build
SRCS += $(STM_COMMON)/Libraries/CMSIS/Device/ST/STM32F30x/Source/Templates/TrueSTUDIO/startup_stm32f30x.s

OBJS = $(SRCS:.c=.o)


.PHONY: debug

all: debug
	$(SIZE) $(PROJ_NAME).elf

debug: CFLAGS += -g -O0 -DDEBUG
debug: $(PROJ_NAME).elf

release: CFLAGS += -O2
release: $(PROJ_NAME).elf

$(PROJ_NAME).elf: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBPATHS) $(LIBS)
	$(OBJCOPY) -O ihex $(PROJ_NAME).elf $(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin

clean:
	find . -name \*.o -type f -delete
	find . -name \*.lst -type f -delete
	rm -f $(PROJ_NAME).elf
	rm -f $(PROJ_NAME).bin
	rm -f $(PROJ_NAME).hex

remake: clean all

# Flash the STM32F4
burn: all
	st-flash write $(PROJ_NAME).bin 0x8000000
