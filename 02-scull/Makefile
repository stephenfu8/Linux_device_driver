ifneq ($(KERNELRELEASE) , )
	obj-m :=scull_char.o
	scull_char-objs := scull.o
else
	CURRENT_PATH :=$(shell pwd)
	KERNEL_VERSION :=$(shell uname -r)
	KERNEL_PATH :=/lib/modules/$(KERNEL_VERSION)/build

ALL:
	make -C $(KERNEL_PATH) M=$(CURRENT_PATH) modules
clean:
	make -C $(KERNEL_PATH) M=$(CURRENT_PATH) clean

.PHONY: install uninstall
install:
	sudo insmod scull_char.ko
uninstall:
	sudo rmmod scull_char
endif
