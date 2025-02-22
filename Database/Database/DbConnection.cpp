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