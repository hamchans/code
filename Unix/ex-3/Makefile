CC = gcc

TARGET = mysh
OBJS = main.o cmd.o gettoken.o

RM = rm -f

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)

.c.o:
	gcc -c $<

$(OBJS): mysh.h

clean:
	$(RM) $(OBJS)

clean_target:
	$(RM) $(TARGET)

clean_all:
	$(RM) $(TARGET) $(OBJS)
