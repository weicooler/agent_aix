#cc main.c init.c comm.c -o Macchiato_aix
#cc main.c comm.c -o Macchiato_aix
cc init.c comm/trace.c comm/comm.c  comm/ipcopr.c comm/socket_srv.c main.c  -lpthread -g  -o a.out
cc init.c comm/trace.c comm/comm.c  comm/ipcopr.c comm/socket_srv.c main.c  -lpthread -g  -o Macchiato_aix

