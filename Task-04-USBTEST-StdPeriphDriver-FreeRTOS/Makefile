# Sources

SRCS = main.c stm32f4xx_it.c system_stm32f4xx.c syscalls.c

# USB
SRCS += usb_bsp_neo.c
#SRCS += usbd_usr.c usbd_cdc_vcp.c usbd_desc.c usb_bsp.c

# Project name
PROJ_NAME=Task-04-USBTEST-StdPeriphDriver-FreeRTOS
OUTPATH=build

###################################################

# Check for valid float argument
# NOTE that you have to run make clan after
# changing these as hardfloat and softfloat are not
# binary compatible
ifneq ($(FLOAT_TYPE), hard)
ifneq ($(FLOAT_TYPE), soft)
override FLOAT_TYPE = hard
#override FLOAT_TYPE = soft
endif
endif

###################################################

BINPATH=
CC=$(BINPATH)arm-none-eabi-gcc
AS=$(BINPATH)arm-none-eabi-as
OBJCOPY=$(BINPATH)arm-none-eabi-objcopy
SIZE=$(BINPATH)arm-none-eabi-size

CFLAGS  = -std=gnu99 -g -O0 -Wall -Lscripts -Tstm32_flash.ld
CFLAGS += -mlittle-endian -mthumb -mthumb-interwork -nostartfiles -mcpu=cortex-m4

ifeq ($(FLOAT_TYPE), hard)
CFLAGS += -fsingle-precision-constant -Wdouble-promotion
CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=hard
else
CFLAGS += -msoft-float
endif

###################################################

vpath %.c Src
vpath %.a lib

ROOT=$(shell pwd)

# Includes
CFLAGS += -IInc -IDrivers/CMSIS/Include -IDrivers/CMSIS/Device/ST/STM32F4xx/Include 
CFLAGS += -IMiddlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS 
CFLAGS += -IMiddlewares/Third_Party/FreeRTOS/Source/include
CFLAGS += -IMiddlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F

# Library paths
LIBPATHS = -LDrivers/STM32F4xx_StdPeriph_Driver 
LIBPATHS += -LMiddlewares/Third_Party/FreeRTOS

#-Llib/USB_Device/Core
#LIBPATHS += -Llib/USB_Device/Class/cdc -Llib/USB_OTG
LIBPATHS += -LMiddlewares/Third_Party/USB_Device/Core 
LIBPATHS += -LMiddlewares/Third_Party/USB_Device/Class/cdc
LIBPATHS += -LMiddlewares/Third_Party/USB_OTG

# Libraries to link
LIBS = -lm -lstdperiph -lfreertos 
LIBS += -lusbdevcore 
#-lusbdevcdc
#-lusbcore
# Extra includes
CFLAGS += -IDrivers/STM32F4xx_StdPeriph_Driver/Inc
CFLAGS += -IMiddlewares/Third_Party/USB_OTG/inc
CFLAGS += -IMiddlewares/Third_Party/USB_Device/Core/inc
CFLAGS += -IMiddlewares/Third_Party/USB_Device/Class/cdc/inc
CFLAGS += -IMiddlewares/Third_Party/Conf

# add startup file to build
SRCS += Src/startup_stm32f4xx.S

OBJS = $(SRCS:.c=.o)
#OBJS += lib/startup_stm32f4xx.o
###################################################

.PHONY: driver middlewares proj

all: driver middlewares proj
	$(SIZE) --format=berkeley $(OUTPATH)/$(PROJ_NAME).elf

driver:
	$(MAKE) -C Drivers FLOAT_TYPE=$(FLOAT_TYPE)

middlewares:
	$(MAKE) -C Middlewares FLOAT_TYPE=$(FLOAT_TYPE)

proj: 	$(OUTPATH)/$(PROJ_NAME).elf

$(OUTPATH)/$(PROJ_NAME).elf: $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBPATHS) $(LIBS)
	$(OBJCOPY) -O ihex $(OUTPATH)/$(PROJ_NAME).elf $(OUTPATH)/$(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(OUTPATH)/$(PROJ_NAME).elf $(OUTPATH)/$(PROJ_NAME).bin
	
clean:
	rm -f *.o
	rm -f $(OUTPATH)/$(PROJ_NAME).elf
	rm -f $(OUTPATH)/$(PROJ_NAME).hex
	rm -f $(OUTPATH)/$(PROJ_NAME).bin
	$(MAKE) clean -C Drivers 
	$(MAKE) clean -C Middlewares 
# Remove this line if you don't want to clean the libs as well
	
