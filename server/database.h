#ifndef DATABASE_H
#define DATABASE_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define LEN_20 20

sqlite3 *db;

void closeDatabase();
void connectDatabase(const char *database); 

int callback(void *NotUsed, int argc, char **argv, char **azColName);
int friends(void *sock, int argc, char **argv, char **azColName);
int check(void *ans, int argc, char **argv, char **azColName);
int select_ID(void *data, int argc, char **argv, char **azColName);

void createTableClients();
void createTableFriends(const char *nik);

void insertIntoTable(const char *table, const char *nik);
void insertIntoClients(const char *nik, const char *pass);

void selectAllClients();
int getID(const char *nik);
void selectAllFriends(int *sock, const char *nik);
int registrationCheck(const char *nik, const char *pass);

int isExistInClients(const char *nik);
int isExistFriend(const char *table, const char *nik);

#endif // DATABASE_H