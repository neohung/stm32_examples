# Sources

SRCS = main.cpp ringbuf.cpp

###################################################

# Project name
PROJ_NAME=test_pc_by_socket
OUTPATH=build

###################################################

BINPATH=
CC=$(BINPATH)gcc
CXX=$(BINPATH)g++
AS=$(BINPATH)as
OBJCOPY=$(BINPATH)objcopy
SIZE=$(BINPATH)size

###################################################

CXXFLAGS  = -O2 -g -Wall -fmessage-length=0
CXXFLAGS +=  -DHAVE_STRUCT_TIMESPEC

###################################################

vpath %.cpp Src
vpath %.a Lib

###################################################

ROOT=$(shell pwd)

# Includes
CXXFLAGS += -IInc 

# Library paths
LIBPATHS = -LLib 

# Libraries to link
LIBS = -lm -lpthreadGC2
LIBS += -lWs2_32
#LIBS += -lusb

# Extra includes
CXXFLAGS += 


# add files to build
SRCS += 

OBJS = $(SRCS:.cpp=.o)

###################################################

.PHONY: proj

all: proj
	$(SIZE) --format=berkeley $(OUTPATH)/$(PROJ_NAME).exe

proj: $(OUTPATH)/$(PROJ_NAME).exe

$(OUTPATH)/$(PROJ_NAME).exe: $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBPATHS) $(LIBS)
	
clean:
	rm -f *.o
	rm -f $(OUTPATH)/$(PROJ_NAME).exe
#$(MAKE) clean -C Drivers 

###################################################
