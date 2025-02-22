#pragma once
#include<sql.h>
#include<sqlext.h>

/*
    DbConnection
*/

class DbConnection
{
    enum
    {
        WVARCHAR_MAX = 4000,
        BINARY_MAX = 8000
    };
public:
    bool Connect(SQLHENV henv, const WCHAR *connectionString);
    void Clear();

    bool Execute(const WCHAR *query);
    bool Fetch();

    int GetRowCount() const;
    void UnBind();

public:
    bool BindParam(int paramIndex, bool *value, SQLLEN *index);
    bool BindParam(int paramIndex, float *value, SQLLEN *index);
    bool BindParam(int paramIndex, double *value, SQLLEN *index);
    bool BindParam(int paramIndex, byte *value, SQLLEN *index);
    bool BindParam(int paramIndex, short *value, SQLLEN *index);
    bool BindParam(int paramIndex, int *value, SQLLEN *index);
    bool BindParam(int paramIndex, long long *value, SQLLEN *index);
    bool BindParam(int paramIndex, TIMESTAMP_STRUCT *value, SQLLEN *index);
    bool BindParam(int paramIndex, const WCHAR *str, SQLLEN *index);
    bool BindParam(int paramIndex, const BYTE *str, int size, SQLLEN *index);

    bool BindCol(int columnIndex, bool *value, SQLLEN *index);
    bool BindCol(int columnIndex, float *value, SQLLEN *index);
    bool BindCol(int columnIndex, double *value, SQLLEN *index);
    bool BindCol(int columnIndex, byte *value, SQLLEN *index);
    bool BindCol(int columnIndex, short *value, SQLLEN *index);
    bool BindCol(int columnIndex, int *value, SQLLEN *index);
    bool BindCol(int columnIndex, long long *value, SQLLEN *index);
    bool BindCol(int columnIndex, TIMESTAMP_STRUCT *value, SQLLEN *index);
    bool BindCol(int columnIndex, WCHAR *str, int size,  SQLLEN *index);
    bool BindCol(int columnIndex, BYTE *str, int size, SQLLEN *index);


private:
    bool BindParam(SQLUSMALLINT paramIndex, SQLSMALLINT cType, SQLSMALLINT sqlType, SQLULEN len, SQLPOINTER ptr, SQLLEN* index);
    bool BindCol(SQLUSMALLINT columIndex, SQLSMALLINT cType, SQLULEN len, SQLPOINTER value, SQLLEN *index);
    void HandleError(SQLRETURN ret);

private:
    SQLHDBC _connection = SQL_NULL_HANDLE;
    SQLHSTMT _statement = SQL_NULL_HANDLE;
};
