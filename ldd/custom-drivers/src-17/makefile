CROSS_COMPILE=arm-linux-gnueabihf-gcc
RM = rm -rf
STANDARD = -std=c99
OPTIMIZATION = -O0
#-save-temps - to save all compiler files
#-Werror - treat all warning as error
CFLAGS = $(STANDARD) -c -g $(OPTIMIZATION) -MD -Wall -pedantic
LDFLAGS =
SRC = $(wildcard *.c)
OBJ = $(addsuffix .o, $(basename $(SRC)))
TARGET = adxl345-app
HOST_IP=192.168.7.2
APP_PATH=~/driver/17

all: $(TARGET)

$(TARGET): $(OBJ)
	@$(CROSS_COMPILE) $(LDFLAGS) $^ -o $@ 
	@echo 'Done!'

%.o: %.c
	@$(CROSS_COMPILE) $(CFLAGS) $< -o $@

include $(wildcard *.d)

clean:
	@$(RM) *.o *.d *.out *.s *.i
	@$(RM) $(TARGET)
	@echo 'Cleaning ... Done!'

copy-app:
	scp $(TARGET) $(USER)@$(HOST_IP):$(APP_PATH)

