#include "database.h"

static int exist = 0;
static int id = 0;

// query result output function
int callback(void *data, int argc, char **argv, char **azColName)
{	
	int i;
	for(i = 0; i < argc; i++)
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	
	exist = atoi(argv[argc - 1]);

	printf("\n");
	return 0;
}

void closeDatabase()
{
	sqlite3_close(db);
}

void connectDatabase(const char *database)
{
	if(sqlite3_open(database, &db))
		printf("Can't open database: %s\n", sqlite3_errmsg(db));

	else printf("Connect database successfully\n");
}

void createTableClients()
{
	char *zErrMsg = 0;
	char *sql;
	
	sql = "CREATE TABLE Clients(" \
		"ID     INTEGER     PRIMARY KEY AUTOINCREMENT," \
          	"Nik    CHAR(20)    NOT NULL," \
		"Pass   CHAR(20)    NOT NULL)";

	if(sqlite3_exec(db, sql, callback, 0, &zErrMsg) != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else printf("Table Clients created successfully\n");

}

void insertIntoClients(const char *nik, const char *pass)
{
	char *zErrMsg = 0;
	char *sql;
	
	sql = (char*)malloc((strlen("INSERT INTO Clients (ID, Nik, Pass) VALUES (NULL, '") + strlen(nik) + strlen("', '") + strlen(pass) + strlen("')"))* sizeof(char));
	strcpy(sql, "INSERT INTO Clients (ID, Nik, Pass) VALUES (NULL, '");
	strcat(sql, nik);
	strcat(sql, "', '");
	strcat(sql, pass);
	strcat(sql, "')");

	int rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	if(rc != SQLITE_OK)
	{
		printf("SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else printf("\"%s\" added to \"Clients\" successfully\n", nik);

	createTableFriends(nik);
}

void selectAllClients()
{
	char *zErrMsg = 0;
	char *sql;

	sql = "SELECT * from Clients";

	if(sqlite3_exec(db, sql, callback, 0, &zErrMsg) != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else printf("Operation done successfully\n");
}

void createTableFriends(const char *nik)
{
	char *zErrMsg = 0;
	char *sql;
	
	sql = (char*)malloc(strlen("CREATE TABLE ") + strlen(nik) + strlen(" (`ID` INTEGER PRIMARY KEY AUTOINCREMENT, `Friends` CHAR(20) NOT NULL)")* sizeof(char));
	
	strcpy(sql, "CREATE TABLE ");
	strcat(sql, nik);
	strcat(sql, " (`ID` INTEGER PRIMARY KEY AUTOINCREMENT, `Friends` CHAR(20) NOT NULL)");

	if(sqlite3_exec(db, sql, callback, 0, &zErrMsg) != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else printf("Table \"%s\" created successfully\n", nik);
}

// the function of adding new friends to the table
void insertIntoTable(const char *table,  const char *nik)
{
	char *zErrMsg = 0;
	char *sql;

	sql = (char*)malloc((strlen("INSERT INTO ") + strlen(table) + strlen("(ID, Friends) VALUES (NULL, '") + strlen(nik) + strlen("')")) * sizeof(char));
	strcpy(sql, "INSERT INTO ");
	strcat(sql, table);
	strcat(sql, "(ID, Friends) VALUES (NULL, '");
	strcat(sql, nik);
	strcat(sql, "')");

	if(sqlite3_exec(db, sql, callback, 0, &zErrMsg) != SQLITE_OK)
	{
		printf("SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else printf("%s added to \"%s\" (Friends) successfully\n", nik, table);
}

// check if the client exists
int isExistInClients(const char *nik)
{
	char *zErrMsg = 0;
	char *sql;
	sql = (char*)malloc((strlen("SELECT EXISTS(SELECT Nik FROM Clients WHERE `Nik` LIKE '") + strlen(nik) + strlen("')")) * sizeof(char));
	strcpy(sql, "SELECT EXISTS(SELECT Nik FROM Clients WHERE `Nik` LIKE '");
	strcat(sql, nik);
	strcat(sql, "')");

	if(sqlite3_exec(db, sql, callback, 0, &zErrMsg) != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else printf("Operation done successfully\n");
	return exist;	
}

void selectAllFriends(int *sock, const char *nik)
{
	char *zErrMsg = 0;
	char *sql;

	sql = (char*)malloc((strlen("SELECT * from ") + strlen(nik)) * sizeof(char));
	strcpy(sql,"SELECT * from ");
	strcat(sql, nik);

	if(sqlite3_exec(db, sql, friends, (void*)sock, &zErrMsg) != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else printf("Operation done successfully\n");
}

// send friend list feature
int friends(void *sock, int argc, char **argv, char **azColName)
{	
	int i;
	
	for(i = 0; i < argc; i++)
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	
	char *buf = (char*)malloc((strlen(argv[argc - 1]) + 1)* sizeof(char));
	strcpy(buf, argv[argc - 1]);
	strcat(buf, "\n");
	send(*((int*)sock), buf, strlen(buf), 0);
	
	printf("\n");
	return 0;
}

int registrationCheck(const char *nik, const char *pass)
{
	char *zErrMsg = 0;
	char *sql;

	sql = (char*)malloc((strlen("SELECT * from Clients WHERE `Nik` = '") + strlen(nik)) + strlen("')") * sizeof(char));
	strcpy(sql,"SELECT * from Clients WHERE `Nik` = '");
	strcat(sql, nik);
	strcat(sql, "'");
	
	char *ans = (char*)malloc((LEN_20 * 2) * sizeof(char));
	
	if(sqlite3_exec(db, sql, check, (void*)ans, &zErrMsg) != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else printf("Operation done successfully\n");

	char nickname[LEN_20];
	char password[LEN_20];
	sscanf(ans, "%s%s", nickname, password);
	if(!strcmp(nickname, nik) && !strcmp(password, pass)) return 1;
	return 0;
}

int check(void *ans, int argc, char **argv, char **azColName)
{	
	int i;
	
	for(i = 0; i < argc; i++)
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	
	strcpy((char*)ans, argv[argc - 2]);
	strcat((char*)ans, " ");
	strcat((char*)ans, argv[argc - 1]);
	printf("\n");
	return 0;
}

int getID(const char *nik)
{
	char *zErrMsg = 0;
	char *sql;

	sql = (char*)malloc((strlen("SELECT ID FROM Clients WHERE `Nik` = '") + strlen(nik) + strlen("'")) * sizeof(char));
	strcpy(sql,"SELECT ID FROM Clients WHERE `Nik` = '");
	strcat(sql, nik);
	strcat(sql, "'");
	if(sqlite3_exec(db, sql, select_ID, 0, &zErrMsg) != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else printf("Operation done successfully\n");
	return id;
}
	
int select_ID(void *data, int argc, char **argv, char **azColName)
{	
	int i;
	for(i = 0; i < argc; i++)
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	
	id = atoi(argv[0]);

	printf("\n");
	return 0;
}

// check if the friend exists
int isExistFriend(const char *table, const char *nik)
{
	char *zErrMsg = 0;
	char *sql;
	sql = (char*)malloc((strlen("SELECT EXISTS(SELECT `Friends` FROM ") + strlen(table) + strlen(" WHERE `Friends` = '") + strlen(nik) + strlen("')")) * sizeof(char));
	strcpy(sql, "SELECT EXISTS(SELECT Friends FROM ");
	strcat(sql, table);
	strcat(sql, " WHERE `Friends` = '");
	strcat(sql, nik);
	strcat(sql, "')");

	if(sqlite3_exec(db, sql, callback, 0, &zErrMsg) != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else printf("Operation done successfully\n");

	return exist;	
}