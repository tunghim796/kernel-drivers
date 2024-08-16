# Root Makefile

.PHONY: all user kernel clean

# Default target, runs both user and kernel Makefiles
all: user kernel

# Target to build user code
user:
	$(MAKE) -C user

# Target to build kernel module
kernel:
	$(MAKE) -C kernel

# Target to clean both user and kernel build artifacts
clean:
	$(MAKE) -C user clean
	$(MAKE) -C kernel clean
