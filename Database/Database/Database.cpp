
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
                     "[Gold] INT NULL,"
                     "[Name] NVARCHAR(50) NULL,"
                     "[CreateDate] DATETIME NULL"
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

            WCHAR name[128] = L"정균리";
            SQLLEN nameLen = 0;

            TIMESTAMP_STRUCT ts = {};
            ts.year = 2025;
            ts.month = 3;
            ts.day = 2;
            SQLLEN tsLen = 0;


            // 넘길 인자 바인딩
            dbConn->BindParam(1, &gold, &len);
            dbConn->BindParam(2, name, &nameLen);
            dbConn->BindParam(3, &ts, &tsLen);

            dbConn->Execute(L"INSERT INTO [dbo].[Gold](Gold, Name, CreateDate) VALUES(?, ?, ?)");
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
        dbConn->BindParam(1, &gold, &len);

        int outId;
        SQLLEN outLen;
        dbConn->BindCol(1, &outId, &outLen);

        int outGold;
        SQLLEN outGoldLen;
        dbConn->BindCol(2, &outGold, &outGoldLen);
        
        WCHAR name[128];
        SQLLEN nameLen = 0;
        dbConn->BindCol(3, name, 128, &nameLen);

        TIMESTAMP_STRUCT outDate;
        SQLLEN dateLen = 0;
        dbConn->BindCol(4, &outDate, &dateLen);

        std::wcout.imbue(std::locale("kor"));

        dbConn->Execute(L"SELECT Id, Gold, Name, CreateDate FROM [dbo].[Gold] Where Gold = (?)");
        while (dbConn->Fetch())
        {
            wcout << "Id: " << outId << ", Gold: " << outGold << ", Name: " << name << endl;
            wcout << "Year: " << outDate.year << "/ Month: " << outDate.month << "/ Day: " << outDate.day << endl;
        }
        connectionPool.Push(dbConn);
    }
}
