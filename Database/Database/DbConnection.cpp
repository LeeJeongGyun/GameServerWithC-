#include<iostream>
#include<Windows.h>
#include "DbConnection.h"

bool DbConnection::Connect(SQLHENV henv, const WCHAR *connectionString)
{
    if (::SQLAllocHandle(SQL_HANDLE_DBC, henv, &_connection) != SQL_SUCCESS)
        return false;

    WCHAR stringBuffer[MAX_PATH] = {0};
    ::wcscpy_s(stringBuffer, connectionString);

    WCHAR resultString[MAX_PATH] = {0};
    SQLSMALLINT resultStringLen = 0;

    SQLRETURN ret = ::SQLDriverConnectW(_connection, NULL, reinterpret_cast<SQLWCHAR *>(stringBuffer), _countof(stringBuffer),
                        reinterpret_cast<SQLWCHAR *>(resultString), _countof(resultString), &resultStringLen,
                        SQL_DRIVER_NOPROMPT);
    if (ret == SQL_ERROR)
    {
        SQLWCHAR sqlState[6] = {0};      // SQL 상태 코드 저장
        SQLINTEGER nativeError = 0;      // 네이티브 오류 코드 저장
        SQLWCHAR messageText[256] = {0}; // 오류 메시지 저장
        SQLSMALLINT textLength = 0;      // 메시지 길이
        // SQL 상태 코드 및 오류 메시지 확인
        std::wcout.imbue(std::locale("kor"));
        SQLGetDiagRecW(SQL_HANDLE_DBC, _connection, 1, sqlState, &nativeError, messageText, 256, &textLength);
        wprintf(L"ODBC Connection Error: [%s] %s (Error Code: %d)\n", sqlState, messageText, nativeError);
        return false;
    }



    if (::SQLAllocHandle(SQL_HANDLE_STMT, _connection, &_statement) != SQL_SUCCESS)
    {
        HandleError(ret);
        return false;
    }
        

    return (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO);
}

void DbConnection::Clear()
{
    if (_connection != SQL_NULL_HANDLE)
    {
        ::SQLFreeHandle(SQL_HANDLE_DBC, _connection);
        _connection = SQL_NULL_HANDLE;
    }

    if (_statement != SQL_NULL_HANDLE)
    {
        ::SQLFreeHandle(SQL_HANDLE_STMT, _statement);
        _statement = SQL_NULL_HANDLE;
    }
}

bool DbConnection::Execute(const WCHAR *query)
{
    SQLRETURN ret = ::SQLExecDirectW(_statement, (SQLWCHAR *)query, SQL_NTSL);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        return true;
    
    HandleError(ret);
    return false;
}

bool DbConnection::Fetch()
{
    SQLRETURN ret = ::SQLFetch(_statement);
    switch (ret)
    {
    case SQL_SUCCESS:
    case SQL_SUCCESS_WITH_INFO:
        return true;
    case SQL_NO_DATA:
        return false;
    case SQL_ERROR:
        HandleError(ret);
        return false;
    }
    
    return true;
}

int DbConnection::GetRowCount() const
{
    SQLLEN rowCount = 0;
    SQLRETURN ret = ::SQLRowCount(_statement, &rowCount);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        return static_cast<int>(rowCount);

    return -1;
}

void DbConnection::UnBind()
{
    ::SQLFreeStmt(_statement, SQL_UNBIND);
    ::SQLFreeStmt(_statement, SQL_RESET_PARAMS);
    ::SQLFreeStmt(_statement, SQL_CLOSE);
}

bool DbConnection::BindParam(int paramIndex, bool *value, SQLLEN *index)
{
    return BindParam(paramIndex, SQL_C_TINYINT, SQL_TINYINT, sizeof(bool), value, index);
}

bool DbConnection::BindParam(int paramIndex, float *value, SQLLEN *index)
{
    // 정수가 아닐때는 size 0으로 밀어도 된다.
    return BindParam(paramIndex, SQL_C_FLOAT, SQL_REAL, 0, value, index);
}

bool DbConnection::BindParam(int paramIndex, double *value, SQLLEN *index)
{
    return BindParam(paramIndex, SQL_C_DOUBLE, SQL_DOUBLE, 0, value, index);
}

bool DbConnection::BindParam(int paramIndex, byte *value, SQLLEN *index)
{
    return BindParam(paramIndex, SQL_C_TINYINT, SQL_TINYINT, sizeof(byte), value, index);
}

bool DbConnection::BindParam(int paramIndex, short *value, SQLLEN *index)
{
    return BindParam(paramIndex, SQL_C_SHORT, SQL_SMALLINT, sizeof(short), value, index);
}

bool DbConnection::BindParam(int paramIndex, int *value, SQLLEN *index)
{
    return BindParam(paramIndex, SQL_C_LONG, SQL_INTEGER, sizeof(short), value, index);
}

bool DbConnection::BindParam(int paramIndex, LONGLONG *value, SQLLEN *index)
{
    return BindParam(paramIndex, SQL_C_SBIGINT, SQL_BIGINT, sizeof(LONGLONG), value, index);
}

bool DbConnection::BindParam(int paramIndex, TIMESTAMP_STRUCT *value, SQLLEN *index)
{
    return BindParam(paramIndex, SQL_C_TYPE_TIMESTAMP, SQL_TYPE_TIMESTAMP, sizeof(TIMESTAMP_STRUCT), value, index);
}

bool DbConnection::BindParam(int paramIndex, const WCHAR *str, SQLLEN *index)
{
    SQLULEN size = static_cast<SQLULEN>((::wcslen(str) + 1) * 2);
    *index = SQL_NTSL;

    if (size > WVARCHAR_MAX)
        return BindParam(paramIndex, SQL_C_WCHAR, SQL_WLONGVARCHAR, size, (SQLPOINTER)str, index);
    else
        return BindParam(paramIndex, SQL_C_WCHAR, SQL_WCHAR, size, (SQLPOINTER)str, index);
}

bool DbConnection::BindParam(int paramIndex, const BYTE *str, int size, SQLLEN *index)
{
    if (str == nullptr)
    {
        *index = SQL_NULL_DATA;
        size = 1;
    }
    else
        *index = size;

    if (size > BINARY_MAX)
        return BindParam(paramIndex, SQL_C_BINARY, SQL_LONGVARBINARY, size, (BYTE*)str, index);
    else
        return BindParam(paramIndex, SQL_C_BINARY, SQL_BINARY, size, (BYTE *)str, index);
}

bool DbConnection::BindCol(int columnIndex, bool *value, SQLLEN *index)
{
    return BindCol(columnIndex, SQL_C_TINYINT, sizeof(bool), value, index);
}

bool DbConnection::BindCol(int columnIndex, float *value, SQLLEN *index)
{
    return BindCol(columnIndex, SQL_C_FLOAT, sizeof(float), value, index);
}

bool DbConnection::BindCol(int columnIndex, double *value, SQLLEN *index)
{
    return BindCol(columnIndex, SQL_C_DOUBLE, sizeof(double), value, index);
}

bool DbConnection::BindCol(int columnIndex, byte *value, SQLLEN *index)
{
    return BindCol(columnIndex, SQL_C_TINYINT, sizeof(byte), value, index);
}

bool DbConnection::BindCol(int columnIndex, short *value, SQLLEN *index)
{
    return BindCol(columnIndex, SQL_C_SHORT, sizeof(short), value, index);
}

bool DbConnection::BindCol(int columnIndex, int *value, SQLLEN *index)
{
    return BindCol(columnIndex, SQL_C_LONG, sizeof(int), value, index);
}

bool DbConnection::BindCol(int columnIndex, long long *value, SQLLEN *index)
{
    return BindCol(columnIndex, SQL_C_SBIGINT, sizeof(LONGLONG), value, index);
}

bool DbConnection::BindCol(int columnIndex, TIMESTAMP_STRUCT *value, SQLLEN *index)
{
    return BindCol(columnIndex, SQL_C_TYPE_TIMESTAMP, sizeof(TIMESTAMP_STRUCT), value, index);
}

bool DbConnection::BindCol(int columnIndex, WCHAR *str, int size, SQLLEN *index)
{
    return BindCol(columnIndex, SQL_C_WCHAR, size, str, index);
}

bool DbConnection::BindCol(int columnIndex, BYTE *str, int size, SQLLEN *index)
{
    return BindCol(columnIndex, SQL_BINARY, size, str, index);
}

bool DbConnection::BindParam(SQLUSMALLINT paramIndex, SQLSMALLINT cType, SQLSMALLINT sqlType, SQLULEN len,
                             SQLPOINTER ptr, SQLLEN *index)
{
    SQLRETURN ret = ::SQLBindParameter(_statement, paramIndex, SQL_PARAM_INPUT, cType, sqlType, len, 0, ptr, 0, index);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        return true;

    HandleError(ret);
    return false;
}

bool DbConnection::BindCol(SQLUSMALLINT columnIndex, SQLSMALLINT cType, SQLULEN len, SQLPOINTER value, SQLLEN *index)
{
    SQLRETURN ret = ::SQLBindCol(_statement, columnIndex, cType, value, len, index);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        return true;

    HandleError(ret);
    return false;
}

void DbConnection::HandleError(SQLRETURN ret)
{
    if (ret == SQL_SUCCESS)
        return;

    SQLSMALLINT index = 1;
    SQLWCHAR sqlState[MAX_PATH] = {0};
    SQLINTEGER nativeErr = 0;
    SQLWCHAR errMsg[MAX_PATH] = {0};
    SQLSMALLINT msgLen = 0;
    SQLRETURN errorRet = 0;

    while (true)
    {
        errorRet = ::SQLGetDiagRecW(SQL_HANDLE_STMT, _connection, index, sqlState, OUT & nativeErr, errMsg,
                                    _countof(errMsg), OUT & msgLen);

        if (errorRet == SQL_NO_DATA)
            break;

        if (errorRet != SQL_SUCCESS && errorRet != SQL_SUCCESS_WITH_INFO)
            break;

        // TODO : Log
        std::wcout.imbue(std::locale("kor"));
        std::wcout << errMsg << std::endl;

        index++;
    }
}