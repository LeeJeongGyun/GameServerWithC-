#include "DbConnectionPool.h"

DbConnectionPool::DbConnectionPool()
{
}

DbConnectionPool::~DbConnectionPool()
{
    Clear();
}

bool DbConnectionPool::Connect(int connectionCount, const WCHAR *connectionString)
{
    {
        // Lock

        // 핸들 얻기
        if (::SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &_environment) != SQL_SUCCESS)
            return false;

        // 버전 연결
        if (::SQLSetEnvAttr(_environment, SQL_ATTR_ODBC_VERSION, reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3), 0) !=
            SQL_SUCCESS)
            return false;

        for (int count = 0; count < connectionCount; ++count)
        {
            DbConnection *connection = new DbConnection();
            if (connection->Connect(_environment, connectionString) == false)
                return false;

            _connections.push(connection);
        }
    }
    
    return true;
}

void DbConnectionPool::Clear()
{
    {
        // Lock
        if (_environment != SQL_NULL_HANDLE)
        {
            ::SQLFreeHandle(SQL_HANDLE_ENV, _environment);
            _environment = SQL_NULL_HANDLE;
        }

        while (_connections.empty() == false)
        {
            DbConnection *connection = _connections.top();
            _connections.pop();
        }
    }
}

DbConnection *DbConnectionPool::Pop()
{
    {
        // Lock
        if (_connections.empty())
            return nullptr;

        DbConnection *connection = _connections.top();
        _connections.pop();

        return connection;
    }
    
}

void DbConnectionPool::Push(DbConnection *connection)
{
    // Lock
    _connections.push(connection);
}
