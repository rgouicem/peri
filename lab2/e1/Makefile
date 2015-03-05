CARD_NUMB	= 24
ROUTER		= 132.227.116.54
LAB		= lab2
LOGIN		= C1J1A

MODULE		= ledbtnC1J1A
KERNELDIR 	= /home/redha/m1/peri/raspberrypi/linux
CROSS_COMPILE 	= bcm2708hardfp-
	
obj-m 		+= $(MODULE).o
default:;	make -C $(KERNELDIR) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) M=$(PWD) modules
clean:; 	make -C $(KERNELDIR) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) M=$(PWD) clean
upload:;	scp $(MODULE).ko test.c pi@$(ROUTER):$(LOGIN)/$(LAB)
