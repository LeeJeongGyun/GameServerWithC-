
#include <iostream>
#include<vector>
#include "DbConnectionPool.h"
using namespace std;

int main()
{
    DbConnectionPool connectionPool;

    bool ret = connectionPool.Connect(1, L"Driver={SQL Server Native Client 11.0};Server=(localdb)\\MSSQLLocalDB;Database=ServerDB;Trusted_Connection=Yes;");

    //CREATE TABLE
    {
        auto query = L"                         "
                     "DROP TABLE IF EXISTS [dbo].[Gold]  "
                     "CREATE TABLE [dbo].[Gold]           "
                     "("
                     "[Id] INT NOT NULL PRIMARY KEY IDENTITY,"
                     "[Gold] INT NULL"
                     ")";

        DbConnection *dbConn = connectionPool.Pop();
        dbConn->Execute(query);
        connectionPool.Push(dbConn);
    }

    // INSERT
    {
        for (int i = 0; i < 3; ++i)
        {
            DbConnection *dbConn = connectionPool.Pop();

            // 기존에 바인딩된 게 있다면 삭제
            dbConn->UnBind();

            int gold = 100;
            SQLLEN len = 0;

            // 넘길 인자 바인딩
            dbConn->BindParam(1, SQL_C_LONG, SQL_INTEGER, sizeof(gold), &gold, &len);

            dbConn->Execute(L"INSERT INTO [dbo].[Gold](Gold) VALUES(?)");
            connectionPool.Push(dbConn);
        }
    }

    // Read
    {
        DbConnection *dbConn = connectionPool.Pop();
        
        // 기존에 바인딩된 게 있다면 삭제
        dbConn->UnBind();

        int gold = 100;
        SQLLEN len = 0;
        // 넘길 인자 바인딩
        dbConn->BindParam(1, SQL_C_LONG, SQL_INTEGER, sizeof(gold), &gold, &len);

        int outId;
        SQLLEN outLen;
        dbConn->BindCol(1, SQL_C_LONG, sizeof(int), &outId, &outLen);

        int outGold;
        SQLLEN outGoldLen;
        dbConn->BindCol(2, SQL_C_LONG, sizeof(int), &outGold, &outGoldLen);
        
        dbConn->Execute(L"SELECT Id, Gold FROM [dbo].[Gold] Where Gold = (?)");
        while (dbConn->Fetch())
        {
            cout << "Id: " << outId << ", Gold: " << outGold << endl;
        }
        connectionPool.Push(dbConn);
    }
}
