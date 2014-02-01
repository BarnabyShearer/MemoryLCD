TARGET=main.hex
EXECUTABLE=main.elf

CC=arm-none-eabi-gcc
CXX=arm-none-eabi-g++
LD=arm-none-eabi-gcc
AR=arm-none-eabi-ar
AS=arm-none-eabi-as
CP=arm-none-eabi-objcopy
OD=arm-none-eabi-objdump

BIN=$(CP) -O ihex

DEFS = -DDEBUG -DUSE_FULL_ASSERT -DUSE_STDPERIPH_DRIVER -DSTM32F4XX -DSTM32F40_41xxx -DHSE_VALUE=8000000 -DUSE_STARTUP_FILES -DARDUINO=100

MCU = cortex-m4
MCFLAGS = -mcpu=$(MCU) -mthumb -mlittle-endian -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb-interwork

STM32_INCLUDES = \
				-Ilibs/ \
				-Ilibs/CMSIS/include/ \
				-Ilibs/STM32F4xx_StdPeriph_Driver/inc/ \

OPTIMIZE = -Os
DEBUG = -g3 -gdwarf-2
WARN = -Wall -Wextra -Wredundant-decls -Wundef -Wshadow -Werror-implicit-function-declaration
CWARN = -Wimplicit-function-declaration -Wstrict-prototypes
CFLAGS = -std=gnu11 $(MCFLAGS) $(OPTIMIZE) $(DEBUG) $(WARN) $(CWARN) $(DEFS) -I./ $(STM32_INCLUDES)
CXXFLAGS = $(MCFLAGS) $(OPTIMIZE) $(DEBUG) $(WARN) $(DEFS) -I./ $(STM32_INCLUDES) -include cstdlib
AFLAGS = $(MCFLAGS)
LDFLAGS = -T ldscripts/libs.ld -T ldscripts/mem.ld -T ldscripts/sections.ld --specs=nano.specs --specs=rdimon.specs

OBJECTS = main.o \
            Arduino.o \
            Print.o \
			MemoryLCD.o \
			libs/Adafruit-GFX-Library/Adafruit_GFX.o \
			libs/_sbrk.o \
			libs/syscalls.o \
			libs/CMSIS/src/startup_cm.o \
			libs/CMSIS/src/startup_stm32f4xx.o \
			libs/CMSIS/src/system_stm32f4xx.o \
			libs/CMSIS/src/vectors_stm32f4xx.o \
			libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_adc.o \
			libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_crc.o \
			libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dbgmcu.o \
			libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dma.o \
			libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_exti.o \
			libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_flash.o \
			libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.o \
			libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_i2c.o \
			libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_iwdg.o \
			libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_pwr.o \
			libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.o \
			libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rtc.o \
			libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_sdio.o \
			libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_spi.o \
			libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_syscfg.o \
			libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_tim.o \
			libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_usart.o \
			libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_wwdg.o \
			libs/STM32F4xx_StdPeriph_Driver/src/misc.o \
			libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_cryp.o \
			libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_hash.o \
			libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rng.o \
			libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_can.o \
			libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dac.o \
			libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dcmi.o \
			libs/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_fsmc.o \

all: $(TARGET)

$(TARGET): $(EXECUTABLE)
	$(CP) -O ihex $^ $@

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CFLAGS) $(LDFLAGS) $^ -o $@

clean:
	rm -f $(TARGET) $(EXECUTABLE) $(OBJECTS)
