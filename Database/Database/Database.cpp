
#include <iostream>
#include<vector>
#include "DbConnectionPool.h"
#include "DBBind.h"
#include "XMLParser.h"
using namespace std;

int main()
{
    XmlNode root;
    XmlParser parser;
    if (parser.ParserFromFile(L"GameDB.xml", root) == false)
        return -1;

    wcout.imbue(std::locale("kor"));

    for (XmlNode table : root.FindNodeList(L"Table"))
    {
        wcout << "Name: " << table.GetStringAttr(L"name") << endl;
        for (XmlNode column : table.FindNodeList(L"Column"))
        {
            wcout << "Name: " << column.GetStringAttr(L"name") << ", ";
            wcout << "Type: " << column.GetStringAttr(L"type") << ", ";
            wcout << "NotNull: " << column.GetBoolAttr(L"notnull") << endl;
        }

        for (XmlNode index : table.FindNodeList(L"Index"))
        {
            wstring indexType = index.GetStringAttr(L"type");
            bool isPrimaryKey = index.FindNode(L"PrimaryKey").IsValid();
            bool uniqueConstraint = index.FindNode(L"UniqueKey").IsValid();

            for (XmlNode column : index.FindNodeList(L"Column"))
            {
                wcout << "id: " << column.GetStringAttr(L"name") << endl;
            }
        }
    }

    for (XmlNode procedure : root.FindNodeList(L"Procedure"))
    {
        wcout << "Name: " << procedure.GetStringAttr(L"name") << endl;
        for (XmlNode node : procedure.FindNodeList(L"Param"))
        {
            wcout << "Name: " << node.GetStringAttr(L"name") << ", ";
            wcout << "Type: " << node.GetStringAttr(L"type") << endl;
        }
    }
}
