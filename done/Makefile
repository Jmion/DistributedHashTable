LDLIBS = -lcheck -lm -pthread
#debug flag
CFLAGS += -g 
CFLAGS += -DDEBUG

all : pps-client-put pps-client-get pps-launch-server pps-list-nodes pps-dump-node pps-client-cat pps-client-substr pps-client-find clean
clean :
	rm *.o


pps-client-put : pps-client-put.c hashtable.o node.o node_list.o client.o network.o system.o error.o args.o
pps-client-get : pps-client-get.c hashtable.o node.o node_list.o client.o network.o system.o error.o args.o
pps-launch-server : pps-launch-server.c system.o hashtable.o error.o
pps-list-nodes : pps-list-nodes.c node.o node_list.o client.o system.o error.o args.o
pps-dump-node : pps-dump-node.c system.o
pps-client-cat : pps-client-cat.c hashtable.o node.o node_list.o client.o network.o system.o error.o args.o
pps-client-substr : pps-client-substr.c hashtable.o node.o node_list.o client.o network.o system.o error.o args.o
pps-client-find : pps-client-find.c hashtable.o node.o node_list.o client.o network.o system.o error.o args.o

