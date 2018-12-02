TARGET = hps_gpio

ALT_DEVICE_FAMILY ?= soc_cv_av

HWLIBS_ROOT = $(SOCEDS_HWLIB)

CFLAGS = -g -Wall -Werror -I$(HWLIBS_ROOT)/include -I$(HWLIBS_ROOT)/include/$(ALT_DEVICE_FAMILY) -D$(ALT_DEVICE_FAMILY)
LDFLAGS = -g -Wall -Werror

CROSS_COMPILE = $(GCC_Linaro)/arm-linux-gnueabihf-
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)gcc
ARCH = arm

IP ?= 127.0.0.1
SSH_USER ?= root

.PHONY: build
build: $(TARGET)

deploy: $(TARGET)
	scp ./$(TARGET) root@$(IP):/root/rafael

run: $(TARGET)
	ssh $(SSH_USER)@$(IP) /root/rafael

$(TARGET): main.o
	$(LD) $(LDFLAGS)   $^ -o $@

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(TARGET) *.a *.o *~
