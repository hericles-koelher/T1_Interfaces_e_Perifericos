# Ação padrão
all: Main Driver

Main:
	gcc -o src/Main src/main.c

# Definição de coisas do driver
obj-m := driver_src/mycalc.o

# Compilação do driver
Driver:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

clean:
	rm src/Main
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean

run:
	./src/Main