CC = gcc

TARGET1 = mydhcp_server
OBJS1 = mydhcp_server.o mydhcps_cmd.o server_client_cmd.o
TARGET2 = mydhcp_client
OBJS2 = mydhcp_client.o mydhcpc_cmd.o server_client_cmd.o

RM = rm -f

$(TARGET1): $(OBJS1)
	$(CC) -o $@ $(OBJS1)
$(TARGET2): $(OBJS2)
	$(CC) -o $@ $(OBJS2)

.c.o:
	gcc -c $<

$(OBJS1): mydhcp.h
$(OBJS2): mydhcp.h

clean:
	$(RM) $(OBJS1) $(OBJS2)

clean_target:
	$(RM) $(TARGET1) $(TARGET2)

clean_all:
	$(RM) $(TARGET1) $(OBJS1) $(TARGET2) $(OBJS2)
