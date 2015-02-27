CC	= gcc
CFLAGS	= -g
OBJS	= task-1.o

task-1: $(OBJS)
	$(CC) $(OBJS) -o task-1

task-1.o: task-1.c
	$(CC) $(CFLAGS) -c task-1.c -o task-1.o
