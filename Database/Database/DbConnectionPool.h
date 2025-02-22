#pragma once
#include<windows.h>
#include "DbConnection.h"
#include<stack>
#include<iostream>
/*
	DbConnectionPool
*/

class DbConnectionPool
{
public:
    DbConnectionPool();
    ~DbConnectionPool();

    bool Connect(int connectionCount, const WCHAR *connectionString);
    void Clear();

    DbConnection *Pop();
    void Push(DbConnection *connection);

private:
    SQLHENV _environment = SQL_NULL_HANDLE;
    std::stack<DbConnection*> _connections;
};
