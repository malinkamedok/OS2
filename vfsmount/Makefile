obj-m += kern_vfsmount.o

build_kern:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

ins:
	sudo insmod kern_vfsmount.ko

rm:
	sudo rmmod kern_vfsmount

build:
	gcc -o user_vfsmount user_vfsmount.c