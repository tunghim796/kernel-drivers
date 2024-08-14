#Makefile
KDIR = /lib/modules/`uname -r`/build
#KDIR = /home/tung/Desktop/zcu111_kernel_source/kernel-source

all :
	$(MAKE) -C $(KDIR) M=`pwd` modules

clean :
	$(MAKE) -C $(KDIR) M=`pwd` clean
