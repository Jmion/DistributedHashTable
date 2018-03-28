LDLIBS = -lm -lcheck -pthread
#debug flag
CFLAGS += -g 
CFLAGS += -DDEBUG

all : pps-client-put pps-client-get pps-launch-server

pps-client-put : pps-client-put.c node.o node_list.o client.o network.o system.o 
pps-client-get : pps-client-get.c node.o node_list.o client.o network.o system.o
pps-launch-server : pps-launch-server.c node.o client.o network.o system.o hashtable.o

node_list.o : node_list.h node_list.c node.o system.o error.o
node.o : node.h node.c system.o
client.o : client.h client.c system.o
network.o : network.h network.c
system.o : system.h system.c
hashtable.o : hashtable.h hashtable.c
error.o : error.h error.c