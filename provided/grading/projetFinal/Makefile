LDLIBS = -lcheck -lm -lrt -pthread -lsubunit -lcrypto
#debug flag
#CFLAGS += -g
#CFLAGS += -DDEBUG
CFLAGS += -Wall

all : pps-client-put pps-client-get pps-launch-server pps-list-nodes pps-dump-node pps-client-cat pps-client-substr pps-client-find test-hashtable


pps-client-put : hashtable.o node.o node_list.o client.o network.o system.o error.o args.o ring.o
pps-client-get : hashtable.o node.o node_list.o client.o network.o system.o error.o args.o ring.o
pps-launch-server : system.o hashtable.o error.o
pps-list-nodes : node.o node_list.o client.o system.o error.o args.o ring.o
pps-dump-node : system.o error.o
pps-client-cat : hashtable.o node.o node_list.o client.o network.o system.o error.o args.o ring.o
pps-client-substr : hashtable.o node.o node_list.o client.o network.o system.o error.o args.o ring.o
pps-client-find : hashtable.o node.o node_list.o client.o network.o system.o error.o args.o ring.o
test-hashtable : hashtable.o error.o

