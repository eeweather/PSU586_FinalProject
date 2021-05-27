LDFLAGS = -g -Wall -std=c11
HEADERS = header.h mem_wb.h

all: clean sim

sim: main.o mem_wb.o
	gcc -o sim main.o mem_wb.o $(LDFLAGS)

main.o: main.c
	gcc -c main.c $(LDFLAGS)

mem_wb.o: mem_wb.c
	gcc -c mem_wb.c $(LDFLAGS)

clean:
	rm -rf sim *.o 

