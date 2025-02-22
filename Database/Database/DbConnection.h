#pragma once
#include<sql.h>
#include<sqlext.h>

/*
    DbConnection
*/
class DbConnection
{
public:
    bool Connect(SQLHENV henv, const WCHAR *connectionString);
    void Clear();

    bool Execute(const WCHAR *query);
    bool Fetch();

    int GetRowCount() const;
    void UnBind();

public:
    bool BindParam(SQLUSMALLINT paramIndex, SQLSMALLINT cType, SQLSMALLINT sqlType, SQLULEN len, SQLPOINTER ptr, SQLLEN* index);
    bool BindCol(SQLUSMALLINT columIndex, SQLSMALLINT cType, SQLULEN len, SQLPOINTER value, SQLLEN *index);
    void HandleError(SQLRETURN ret);

private:
    SQLHDBC _connection = SQL_NULL_HANDLE;
    SQLHSTMT _statement = SQL_NULL_HANDLE;
};
