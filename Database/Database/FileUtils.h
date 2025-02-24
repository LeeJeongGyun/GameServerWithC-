#pragma once

/*
    FileUtil
*/

using namespace std;

class FileUtils
{
public:
    static vector<BYTE> ReadFile(const WCHAR *path);
    static std::wstring Convert(string str);
};
