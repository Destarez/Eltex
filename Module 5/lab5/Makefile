
obj-m += netlink.o
PWD:= $(CURDIR)

all:
	gcc userspace.c -o userspace
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean