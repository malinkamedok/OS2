obj-m += kern_bpf.o

build_kern:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

ins:
	sudo insmod kern_bpf.ko

rm:
	sudo rmmod kern_bpf

build:
	gcc -o user_bpf user_bpf.c