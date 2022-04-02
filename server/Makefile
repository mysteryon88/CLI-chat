 CC=gcc

.PHONY: default build run
default: build run

build: server.c
	$(CC) -Wall server.c list.c database.c -o server.out -pthread -Iinclude -Llib -lsqlite3 