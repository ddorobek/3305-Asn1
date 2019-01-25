CC=gcc
CFLAGS=-c
CMATH=-lm

all: clean myOS 

myOS: myOS.o shell.o helper.o
	$(CC) -o $@ $^ $(CMATH)

myOS.o: myOS.c myOS.h
	$(CC) $(CFLAGS) -o $@ $<

shell.o: shell.c shell.h
	$(CC) $(CFLAGS) -o $@ $< 

helper.o: helper.c helper.h 
	$(CC) $(CFLAGS) -o $@ $<

test: clean myOS
	bash ./tests/run.sh

clean:
	rm -f *.o myOS report.txt