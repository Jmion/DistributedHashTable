LDLIBS = -lcheck -lm -pthread

all : pps-client-put pps-client-get pps-launch-server

pps-client-put : pps-client-put.c node.o client.o network.o system.o
pps-client-get : pps-client-get.c node.o client.o network.o system.o
pps-launch-server : pps-launch-server.c node.o client.o network.o system.o hashtable.o


node.o : node.h node.c system.o
client.o : client.h client.c system.o
network.o : network.h network.c
system.o : system.h system.c
hashtable.o : hashtable.h hashtable.c