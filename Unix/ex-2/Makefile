CC = gcc

TARGET = bufcache
OBJS = main.o getblk.o getargs.o proc.o

RM = rm -f

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)

.c.o:
	gcc -c $<

$(OBJS): buf.h

clean:
	$(RM) $(OBJS)

clean_target:
	$(RM) $(TARGET)

clean_all:
	$(RM) $(TARGET) $(OBJS)
