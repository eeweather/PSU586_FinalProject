LDFLAGS = -g -Wall -std=c11
HEADERS = header.h mem_wb.h

all: clean sim

sim: main.o if.o id.o ie.o mem_wb.o formatting.o
	gcc -o sim main.o if.o id.o ie.o mem_wb.o formatting.o $(LDFLAGS)

main.o: main.c
	gcc -c main.c $(LDFLAGS)

if.o: if.c
	gcc -c if.c $(LDFLAGS)

id.o: id.c
	gcc -c id.c $(LDFLAGS)

ie.o: ie.c
	gcc -c ie.c $(LDFLAGS)

mem_wb.o: mem_wb.c
	gcc -c mem_wb.c $(LDFLAGS)

formatting.o: formatting.c
	gcc -c formatting.c $(LDFLAGS)

clean:
	rm -rf sim *.o 

