CC = gcc
CFLAGS = -g -Wall

shell: main.o exec.o cmds.o
			$(CC) $(CFLAGS) -o shell main.o exec.o cmds.o
exec.o: exec.c exec.h
			$(CC) $(CFLAGS) -c exec.c
cmds.o: cmds.c exec.h
			$(CC) $(CFLAGS) -c cmds.c
main.o: main.c exec.h
			$(CC) $(CFLAGS) -c main.c
clean:
	rm -f shell *.o
