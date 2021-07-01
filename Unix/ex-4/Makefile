CC = gcc

TARGET = mydhcp
OBJS = main.o mydhcpc.o mydhcps.o

RM = rm -f

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)

.c.o:
	gcc -c $<

$(OBJS): mydhcp.h

clean:
	$(RM) $(OBJS)

clean_target:
	$(RM) $(TARGET)

clean_all:
	$(RM) $(TARGET) $(OBJS)
