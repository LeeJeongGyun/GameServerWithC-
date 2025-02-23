#pragma once
#include "DbConnection.h"
#include<Windows.h>

template<int N>
struct FullBits
{
    enum
    {
        value = (1 << (N - 1) | FullBits<N - 1>::value)
    };
};

template<>
struct FullBits<1>
{
    enum
    {
        value = 1
    };
};

template <> 
struct FullBits<0>
{
    enum
    {
        value = 0
    };
};

template<int ParamCount, int ColumnCount>
class DBBind
{
public:
    DBBind(DbConnection &dbConnection, const WCHAR *query) : _dbConnection(dbConnection), _query(query)
    {
        ::memset(_paramIndex, 0, sizeof(_paramIndex));
        ::memset(_columnIndex, 0, sizeof(_columnIndex));
        _paramFlag = _columnFlag = 0;
        dbConnection.UnBind();
    }

    bool Validate()
    {
        return _paramFlag == FullBits<ParamCount>::value && _columnFlag == FullBits<ColumnCount>::value;
    }

    bool Execute()
    {
        if (Validate() == false)
            return false;

        _dbConnection.Execute(_query);
        return true;
    }

    bool Fetch()
    {
        return _dbConnection.Fetch();
    }

public:
    template<typename T> 
    void BindParam(int idx, T &value)
    {
        _dbConnection.BindParam(idx + 1, &value, &_paramIndex[idx]);
        _paramFlag |= (1 << idx);
    }

    template<typename T, int N>
    void BindParam(int idx, T(&value)[N])
    {
        _dbConnection.BindParam(idx + 1, (const BYTE *)value, sizeof(T) * N, &_paramIndex[idx]);
        _paramFlag |= (1 << idx);
    }

    template <typename T> 
    void BindParam(int idx, T* value, int N)
    {
        _dbConnection.BindParam(idx + 1, (const BYTE *)value, sizeof(T) * N, &_paramIndex[idx]);
        _paramFlag |= (1 << idx);
    }

    void BindParam(int idx, const WCHAR *value)
    {
        _dbConnection.BindParam(idx + 1, value, &_paramIndex[idx]);
        _paramFlag |= (1 << idx);
    }

    template <typename T> 
    void BindCol(int idx, T &value)
    {
        _dbConnection.BindCol(idx + 1, &value, &_columnIndex[idx]);
        _columnFlag |= (1 << idx);
    }

    template <int N> 
    void BindCol(int idx, WCHAR (&value)[N])
    {
        _dbConnection.BindCol(idx + 1, value, N - 1, &_columnIndex[idx]);
        _columnFlag |= (1 << idx);
    }

    void BindCol(int idx, WCHAR* value, int len)
    {
        _dbConnection.BindCol(idx + 1, value, len - 1, &_columnIndex[idx]);
        _columnFlag |= (1 << idx);
    }

    template <typename T, int N> 
    void BindCol(int idx, T (&value)[N])
    {
        _dbConnection.BindCol(idx + 1, value, sizeof(T) * N, &_columnIndex[idx]);
        _columnFlag |= (1 << idx);
    }

protected:
    DbConnection &_dbConnection;
    const WCHAR *_query;
    SQLLEN _paramIndex[ParamCount > 0 ? ParamCount : 1];
    SQLLEN _columnIndex[ColumnCount > 0 ? ColumnCount : 1];
    int _paramFlag;
    int _columnFlag;
};