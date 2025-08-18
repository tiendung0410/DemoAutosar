# Makefile for AUTOSAR ECU1 Project

# Compiler & Flags
CC      := gcc
CFLAGS  := -Wall -Werror -IInc -IConfig  -IEcuAbstraction/CanIf  -IEcuAbstraction/IoHwAb  -IEcuAbstraction/Gpt -IMCAL/Can -IMCAL/Dio -IMCAL/Gpt -IOS -IRTE -IServices/Com -IServices/PduR -IApp 

# Project folders
SRC_DIRS := App Config EcuAbstraction/IoHwAb EcuAbstraction/CanIf EcuAbstraction/Gpt MCAL/Can MCAL/Dio MCAL/Gpt OS RTE Services/Com Services/PduR

# Gather all source files (.c)
SRCS := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
SRCS += main.c
# Generate object files in Out/
OBJS := $(patsubst %.c,Out/%.o,$(SRCS))

# Output binary
TARGET := Out/autosar_ecu1.elf

# Default rule
all: prepare $(TARGET)

# Create Out/ structure
prepare:
	mkdir -p Out
	@for dir in $(SRC_DIRS); do mkdir -p Out/$$dir; done

# Linking
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ -pthread -lrt -lgpiod

# Compiling source to objects
Out/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ -pthread -lrt -lgpiod

# Clean build artifacts
clean:
	rm -rf Out

.PHONY: all clean prepare
