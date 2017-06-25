# Build all libraries

# Check for valid float argument
ifneq ($(FLOAT_TYPE), hard)
ifneq ($(FLOAT_TYPE), soft)
override FLOAT_TYPE = hard
#override FLOAT_TYPE = soft
endif
endif

all:
	$(MAKE) -C Third_Party FLOAT_TYPE=$(FLOAT_TYPE)

clean:
	$(MAKE) clean -C Third_Party
