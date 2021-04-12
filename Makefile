# Ação padrão
all: Main Driver

Main:
	gcc -o user_src/Main user_src/main.c

# Definição de coisas do driver
obj-m := driver_src/mycalc.o

# Compilação do driver
Driver:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

clean:
	rm user_src/Main
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean

run:
	./user_src/Main

install:
	insmod driver_src/mycalc.ko

uninstall:
	rmmod mycalc