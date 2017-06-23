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

vpath %.c src

CFLAGS  = -g -O2 -Wall
CFLAGS += -mlittle-endian -mthumb -mthumb-interwork -mcpu=cortex-m4

ifeq ($(FLOAT_TYPE), hard)
CFLAGS += -fsingle-precision-constant -Wdouble-promotion
CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=hard
else
CFLAGS += -msoft-float
endif

CFLAGS += -ffreestanding -nostdlib

# Includes
CFLAGS += -ISource/include -ISource/portable/GCC/ARM_CM4F -I../../../Inc -I../../../Drivers/CMSIS/Include

# Sources

SRCS = \
Source/CMSIS_RTOS/cmsis_os.c \
Source/tasks.c \
Source/queue.c \
Source/list.c \
Source/portable/MemMang/heap_4.c \
Source/portable/GCC/ARM_CM4F/port.c \
	

OBJS = $(SRCS:.c=.o)
LIBNAME = libfreertos.a

all: $(LIBNAME)

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $^

$(LIBNAME): $(OBJS)
	$(AR) -r $@ $(OBJS)

clean:
	rm -f $(OBJS) $(LIBNAME)
	