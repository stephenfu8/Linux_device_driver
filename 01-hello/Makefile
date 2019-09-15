ifneq ($(KERNELRELEASE) , )
    obj-m :=mytest.o
    mytest-objs :=helloworld.o
else 
    CURRENT_PATH :=$(shell pwd)
	KERNEL_VERSION :=$(shell uname -r)
    KERNEL_PATH :=/lib/modules/$(KERNEL_VERSION)/build

default:
	make -C $(KERNEL_PATH) M=$(CURRENT_PATH) modules

clean:
	make -C $(KERNEL_PATH) M=$(CURRENT_PATH) clean
  
endif



