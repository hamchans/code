CC = gcc

TARGET1 = myftpd
OBJS1 = myftpd.o myftpd_cmd.o tcps_cmd.o getargs.o cmd.o
TARGET2 = myftpc
OBJS2 = myftpc.o myftpc_cmd.o tcpc_cmd.o getargs.o cmd.o

RM = rm -f

$(TARGET1): $(OBJS1)
	$(CC) -o $@ $(OBJS1)
$(TARGET2): $(OBJS2)
	$(CC) -o $@ $(OBJS2)

.c.o:
	gcc -c $<

$(OBJS1): myftp.h
$(OBJS2): myftp.h

clean:
	$(RM) $(OBJS1) $(OBJS2)

clean_target:
	$(RM) $(TARGET1) $(TARGET2)

clean_all:
	$(RM) $(TARGET1) $(OBJS1) $(TARGET2) $(OBJS2)
