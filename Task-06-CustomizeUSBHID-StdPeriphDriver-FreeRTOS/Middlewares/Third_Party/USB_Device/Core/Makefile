BINPATH=

CC=$(BINPATH)arm-none-eabi-gcc
AR=$(BINPATH)arm-none-eabi-ar

###################################################

# Check for valid float argument
ifneq ($(FLOAT_TYPE), hard)
ifneq ($(FLOAT_TYPE), soft)
override FLOAT_TYPE = hard
#override FLOAT_TYPE = soft
endif
endif

###################################################

CFLAGS  = -g -O0 -Wall
CFLAGS += -mlittle-endian -mthumb -mthumb-interwork -mcpu=cortex-m4

ifeq ($(FLOAT_TYPE), hard)
CFLAGS += -fsingle-precision-constant -Wdouble-promotion
CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=hard
else
CFLAGS += -msoft-float
endif

CFLAGS += -ffreestanding -nostdlib

# Includes
CFLAGS += -Iinc -I../../../../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../../../../Drivers/CMSIS/Include
CFLAGS += -I../../Conf -I../../USB_OTG/inc

# Sources
vpath %.c src
SRCS = usbd_core.c usbd_ioreq.c usbd_req.c

vpath %.c ../../USB_OTG/src/
SRCS += usb_core.c usb_dcd.c usb_dcd_int.c usb_otg.c usb_hcd.c usb_hcd_int.c
#SRCS += ../../USB_OTG/src/usb_core.c ../../USB_OTG/src/usb_dcd.c ../../USB_OTG/src/usb_dcd_int.c ../../USB_OTG/src/usb_otg.c ../../USB_OTG/src/usb_hcd.c ../../USB_OTG/src/usb_hcd_int.c

OBJS = $(SRCS:.c=.o)
LIBNAME = libusbdevcore.a

all: $(LIBNAME)

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $^

$(LIBNAME): $(OBJS)
	$(AR) -r $@ $(OBJS)

clean:
	rm -f $(OBJS) $(LIBNAME)
	